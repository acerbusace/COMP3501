#include <stdexcept>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "laser.h"

namespace game {

Laser::Laser(std::string name, Resource *geometry, Resource *material, Resource *texture) : SceneNode(name, geometry, material, texture) {
	speed_ = 0;
	dist_ = 250;
	damage_ = 25;
}


Laser::~Laser() {
}


void Laser::Update(double delta_time) {
	Translate(glm::vec3(0.0, 0.0, -speed_ * delta_time));
	dist_ -= speed_ * delta_time;
}


float Laser::GetDamage() {
	return damage_;
}


bool Laser::done() {
	if (dist_ < 0)
		return true;
	return false;
}


void Laser::SetInitPos(glm::vec3 pos) {
	init_pos_ = pos;
}


void Laser::SetSpeed(float speed) {
	speed_ = speed;
}


void Laser::SetDamage(float damage) {
	damage_ = damage;
}


std::vector<glm::vec3> Laser::GetLaserPoints() {
	std::vector<glm::vec3> points;
	points.push_back(glm::vec3(-0.5, -0.5, -0.5));
	points.push_back(glm::vec3(0.5, -0.5, -0.5));
	points.push_back(glm::vec3(0.5, 0.5, -0.5));
	points.push_back(glm::vec3(-0.5, 0.5, -0.5));
	points.push_back(glm::vec3(-0.5, -0.5, 0.5));
	points.push_back(glm::vec3(0.5, -0.5, 0.5));
	points.push_back(glm::vec3(0.5, 0.5, 0.5));
	points.push_back(glm::vec3(-0.5, 0.5, 0.5));

    glm::mat4 scaling = glm::scale(glm::mat4(1.0), scale_);
	for (int i = 0; i < points.size(); ++i) {
		points[i] = (glm::vec3) (getTransf() * scaling * glm::vec4(points[i], 1.0));
	}

	return points;
}


glm::mat4 Laser::getTransf() {
    // World transformation
	SetOrientation(glm::normalize(GetOrientation()));
    glm::mat4 rotation = glm::mat4_cast(GetOrientation());
    glm::mat4 translation = glm::translate(glm::mat4(1.0), GetPosition());
    glm::mat4 init_trans = glm::translate(glm::mat4(1.0), init_pos_);

	glm::mat4 transf = init_trans * rotation * translation;

	if (GetParent())
		transf = GetParent()->getTransf() * transf;

	return transf;
}
            
} // namespace game
