#include <stdexcept>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <time.h>
#include <algorithm>

#include "scene_node.h"
#include "camera.h"
#include "misc.h"

namespace game {

SceneNode::SceneNode(std::string name, Resource *geometry, Resource *material, Resource *texture) {

    // Set name of scene node
	setName(name);
	parent = NULL;

    // Set geometry
	setGeometry(geometry);

	// Set material
	setMaterial(material);

	// Set texture
	if (texture)
		texture_ = texture->GetResource();
	else
		texture_ = 0;

    // Other attributes
    scale_ = glm::vec3(1.0, 1.0, 1.0);
	reset_ = 4;
    start_time_ = glfwGetTime();
	curr_time_ = 0;
	color_ = glm::vec3((float) rand() / RAND_MAX, (float) rand() / RAND_MAX, (float) rand() / RAND_MAX);
	rad_ = 0;
}

void SceneNode::SetReset(float reset) {
	reset_ = reset;
    start_time_ = glfwGetTime();
	curr_time_ = 0;
}

float SceneNode::GetRad() {
	return 1.0 * curr_time_;
}

void SceneNode::SetColor(glm::vec3 color){

    color_ = color;
}


SceneNode::~SceneNode(){
}

bool SceneNode::setGeometry(Resource *geometry) {
	// Set geometry
	if (geometry) {
		geoType = geometry->GetType();
		if (geoType == PointSet) {
			mode_ = GL_POINTS;
		}
		else if (geoType == Mesh || geoType == SingleMesh) {
			mode_ = GL_TRIANGLES;
		}
		else {
			throw(std::invalid_argument(std::string("Invalid type of geometry")));
		}

		array_buffer_ = geometry->GetArrayBuffer();
		element_array_buffer_ = geometry->GetElementArrayBuffer();
		size_ = geometry->GetSize();
		return true;
	}
	return false;
}

bool SceneNode::setMaterial(Resource *material) {
	if (material) {
		// Set material (shader program)
		if (material->GetType() != Material) {
			throw(std::invalid_argument(std::string("Invalid type of material")));
		}
		material_ = material->GetResource();
		return true;
	}
	return false;
}

bool SceneNode::setTexture(Resource *texture) {
	// Set texture
	if (texture) {
		texture_ = texture->GetResource();
		return true;
	}
	texture_ = 0;
	return false;
}

const std::string SceneNode::GetName(void) const {

    return name_;
}

void SceneNode::setName(std::string name) {

	name_ = name;
}


glm::vec3 SceneNode::GetPosition(void) const {

    return position_;
}


glm::quat SceneNode::GetOrientation(void) const {

    return orientation_;
}


glm::vec3 SceneNode::GetScale(void) const {

    return scale_;
}


void SceneNode::SetPosition(glm::vec3 position){

    position_ = position;
}


void SceneNode::SetOrientation(glm::quat orientation){

    orientation_ = orientation;
}


void SceneNode::SetScale(glm::vec3 scale){

    scale_ = scale;
}


void SceneNode::Translate(glm::vec3 trans){

    position_ += trans;
}


void SceneNode::Rotate(glm::quat rot){

    orientation_ *= rot;
}

void SceneNode::SetExpDamage(float damage) {
	expDamage_ = damage;
}

float SceneNode::GetExpDamage() {
	return expDamage_;
}


void SceneNode::Scale(glm::vec3 scale){

    scale_ *= scale;
}


GLenum SceneNode::GetMode(void) const {

    return mode_;
}


GLuint SceneNode::GetArrayBuffer(void) const {

    return array_buffer_;
}


GLuint SceneNode::GetElementArrayBuffer(void) const {

    return element_array_buffer_;
}


GLsizei SceneNode::GetSize(void) const {

    return size_;
}


GLuint SceneNode::GetMaterial(void) const {

    return material_;
}

SceneNode * SceneNode::GetParent(void)
{
	return parent;
}


void SceneNode::Draw(Camera *camera, glm::vec3 light_pos){
	if (name_ == "Wall") {
		std::cout << "it should be drawing!!!" << std::endl;
	}

    // Select proper material (shader program)
    glUseProgram(material_);

    // Set geometry to draw
    glBindBuffer(GL_ARRAY_BUFFER, array_buffer_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_array_buffer_);

    // Set globals for camera
    camera->SetupShader(material_, light_pos);

    // Set world matrix and other shader input variables
    SetupShader(material_, light_pos);

    // Draw geometry
	if (geoType == Mesh)
		glDrawElements(mode_, size_, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(mode_, 0, size_);

	for each (SceneNode *child in children) {
		child->Draw(camera, light_pos);
	}
}

glm::vec3 SceneNode::getPos() {
	return glm::vec3(getTransf() * glm::vec4(0.0, 0.0, 0.0, 1.0));
}

void SceneNode::Update(double delta_time){
    // Do nothing for this generic type of scene node
}

bool SceneNode::done() {
	if (curr_time_ > reset_)
		return true;
	return false;
}

void SceneNode::RemoveChildren() {
	children.clear();
}

glm::mat4 SceneNode::getTransf() {
    // World transformation
	orientation_ = glm::normalize(orientation_);
    glm::mat4 rotation = glm::mat4_cast(orientation_);
    glm::mat4 translation = glm::translate(glm::mat4(1.0), position_);

	glm::mat4 transf = translation * rotation;

	if (GetParent())
		transf = parent->getTransf() * transf;

	return transf;
}

void SceneNode::SetupShader(GLuint program, glm::vec3 light_pos){

    // Set attributes for shaders

	GLint vertex_att = glGetAttribLocation(program, "vertex");
	glVertexAttribPointer(vertex_att, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(vertex_att);

	GLint normal_att = glGetAttribLocation(program, "normal");
	glVertexAttribPointer(normal_att, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(normal_att);

	GLint color_att = glGetAttribLocation(program, "color");
	glVertexAttribPointer(color_att, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(color_att);

	GLint tex_att = glGetAttribLocation(program, "uv");
	glVertexAttribPointer(tex_att, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void *)(9 * sizeof(GLfloat)));
	glEnableVertexAttribArray(tex_att);

    // World transformation
    glm::mat4 scaling = glm::scale(glm::mat4(1.0), scale_);
	glm::mat4 transf = getTransf() * scaling;

    GLint world_mat = glGetUniformLocation(program, "world_mat");
    glUniformMatrix4fv(world_mat, 1, GL_FALSE, glm::value_ptr(transf));

    // Normal matrix
    glm::mat4 normal_matrix = glm::transpose(glm::inverse(transf));
    GLint normal_mat = glGetUniformLocation(program, "normal_mat");
    glUniformMatrix4fv(normal_mat, 1, GL_FALSE, glm::value_ptr(normal_matrix));

	// Texture
	if (texture_) {
		GLint tex = glGetUniformLocation(program, "texture_map");
		glUniform1i(tex, 0); // Assign the first texture to the map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_); // First texture we bind
												// Define texture interpolation
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

    // Timer
    GLint timer_var = glGetUniformLocation(program, "timer");
    curr_time_ = glfwGetTime() - start_time_;
    glUniform1f(timer_var, curr_time_);

    // Reset Timer
    GLint reset_var = glGetUniformLocation(program, "reset");
    glUniform1f(reset_var, reset_);

	// Color
    GLint color_var = glGetUniformLocation(program, "uniColor");
    glUniform3f(color_var, color_.x, color_.y, color_.z);

    GLint light_pos_var = glGetUniformLocation(program, "uni_light_pos");
    glUniform3f(light_pos_var, light_pos.x, light_pos.y, light_pos.z);
}

void SceneNode::setParent(SceneNode *prt) {
	parent = prt;
}

void SceneNode::addChild(SceneNode *child) {
	child->setParent(this);
	children.push_back(child);
}

float SceneNode::GetRadius() {
	return fmax(fmax(scale_.x/2, scale_.y/2), scale_.z/2);
}

} // namespace game;
