#include <stdexcept>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "missile.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace game {

Missile::Missile(std::string name, Resource *geometry, Resource *material, Resource *texture) : SceneNode(name, geometry, material, texture) {
	speed_ = 0;
	t = 0;
}


Missile::~Missile(){
}

void Missile::Update(double delta_time){
	t += delta_time / 2;

	if (t < 1) {
		// Spline evaluation
		float p1w = (1 - t)*(1 - t)*(1 - t);
		float p2w = 3 * t*(1 - t)*(1 - t);
		float p3w = 3 * t*t*(1 - t);
		float p4w = t*t*t;
		prevBt = Bt;
		Bt = p1w*point1_ + p2w*point2_ + p3w*point3_ + p4w*point4_;
		SetPosition(Bt - init_pos_);
		SetOrientation(glm::conjugate(glm::toQuat(glm::lookAt(prevBt, Bt, glm::vec3(0.0, 1.0, 0.0)))));
		Rotate(glm::angleAxis((float)glm::radians(90.0f), glm::vec3(-1, 0, 0)));
	}
	//Translate(glm::vec3(0.0, 0.0, -speed_ * delta_time));
    //Rotate(angm_);

}

bool Missile::done() {
	if (t < 1)
		return false;
	return true;
}

void Missile::SetSpeed(float speed) {
	speed_ = speed;
}

void Missile::SetInitPos(glm::vec3 pos) {
	init_pos_ = pos;
	prevBt = init_pos_;
	Bt = init_pos_;
}

void Missile::setPoints(glm::vec3 forward, glm::vec3 up) {
	float up_factor = 4;
	float forward_factor = 500;
	point1_ = init_pos_;
	point2_ = init_pos_ + up * up_factor;
	point3_ = point2_ + forward * (forward_factor / 2);
	point4_ = point3_ - up * up_factor;
}

std::vector<glm::vec3> Missile::GetMissilePoints() {
	std::vector<glm::vec3> points;
	points.push_back(glm::vec3(-0.5, 0.0, -0.5));
	points.push_back(glm::vec3(0.5, 0.0, -0.5));
	points.push_back(glm::vec3(0.5, 5.0, -0.5));
	points.push_back(glm::vec3(-0.5, 5.0, -0.5));
	points.push_back(glm::vec3(-0.5, 0.0, 0.5));
	points.push_back(glm::vec3(0.5, 0.0, 0.5));
	points.push_back(glm::vec3(0.5, 5.0, 0.5));
	points.push_back(glm::vec3(-0.5, 5.0, 0.5));

    glm::mat4 scaling = glm::scale(glm::mat4(1.0), scale_);
	for (int i = 0; i < points.size(); ++i) {
		points[i] = (glm::vec3) (getTransf() * scaling * glm::vec4(points[i], 1.0));
	}

	return points;
}

glm::mat4 Missile::getTransf() {
    // World transformation
	SetOrientation(glm::normalize(GetOrientation()));
    glm::mat4 rotation = glm::mat4_cast(GetOrientation());
    glm::mat4 translation = glm::translate(glm::mat4(1.0), GetPosition());
    glm::mat4 init_trans = glm::translate(glm::mat4(1.0), init_pos_);

	glm::mat4 transf = init_trans * translation * rotation;

	if (GetParent())
		transf = GetParent()->getTransf() * transf;

	return transf;
}
            
} // namespace game
