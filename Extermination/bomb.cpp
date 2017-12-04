#include <stdexcept>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "bomb.h"

namespace game {

Bomb::Bomb(std::string name, Resource *geometry, Resource *material, Resource *texture) : SceneNode(name, geometry, material, texture) {
	speed_ = 0;
	radius_ = 5;
	explode_ = false;
}


Bomb::~Bomb(){
}

void Bomb::Update(double delta_time){
	if (explode_ == false) {
		Translate(glm::vec3(0.0, speed_ * delta_time, 0));
		timer_ -= delta_time;
		if (timer_ < 0) {
			explode_ = true;
		}
		//Rotate(angm_);
	}
}

void Bomb::SetSpeed(float speed) {
	speed_ = speed;
}

void Bomb::SetTimer(float timer)
{
	timer_ = timer;
}

bool Bomb::Explode()
{
	return explode_;
}
            
} // namespace game
