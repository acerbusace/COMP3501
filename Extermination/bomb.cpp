#include <stdexcept>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "bomb.h"

namespace game {

Bomb::Bomb(std::string name, Resource *geometry, Resource *material, Resource *texture) : SceneNode(name, geometry, material, texture) {
	speed_ = 0;
}


Bomb::~Bomb(){
}

void Bomb::Update(double delta_time){
	Translate(glm::vec3(0.0, 0.0, -speed_ * delta_time));
	timer_ -= delta_time;
    //Rotate(angm_);
}

void Bomb::SetSpeed(float speed) {
	speed_ = speed;
}

void Bomb::SetInitPos(glm::vec3 pos) {
	init_pos_ = pos;
}

void Bomb::SetTimer(float timer)
{
	timer_ = timer;
}

glm::mat4 Bomb::getTransf() {
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
