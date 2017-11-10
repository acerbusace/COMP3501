#include <stdexcept>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "laser.h"

namespace game {

Laser::Laser(const std::string name, const Resource *geometry, const Resource *material, Resource *texture) : SceneNode(name, geometry, material, texture) {
	speed_ = 0;
}


Laser::~Laser(){
}

void Laser::Update(double delta_time){
	Translate(glm::vec3(0.0, 0.0, -speed_ * delta_time));
    //Rotate(angm_);
}

void Laser::SetSpeed(float speed) {
	speed_ = speed;
}

void Laser::SetInitPos(glm::vec3 pos) {
	init_pos_ = pos;
}

glm::mat4 Laser::getTransf() {
    // World transformation
	orientation_ = glm::normalize(orientation_);
    glm::mat4 rotation = glm::mat4_cast(orientation_);
    glm::mat4 translation = glm::translate(glm::mat4(1.0), position_);
    glm::mat4 init_trans = glm::translate(glm::mat4(1.0), init_pos_);

	glm::mat4 transf = init_trans * rotation * translation;

	if (parent)
		transf = parent->getTransf() * transf;

	return transf;
}
            
} // namespace game
