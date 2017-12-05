#include "player.h"
#include <iostream>

namespace game {

Player::Player(std::string name, Resource *geometry, Resource *material, Resource *texture) : SceneNode(name, geometry, material, texture) {
	first_person_ = false;
	health_ = 1000;
}


Player::~Player(){
}

void Player::Draw(Camera *camera)
{
	if (first_person_ == false) {
		SceneNode::Draw(camera);
	}
	else {

	}
}

bool Player::takeDamage(float damage) {
	health_ -= damage;
	std::cout << "Player Health: " << health_ << std::endl;

	if (health_ <= 0)
		return true;
	return false;
}

bool Player::get_first_person()
{
	return first_person_;
}

void Player::toggle_first_person()
{
	first_person_ = (first_person_ == true) ? false : true;
}

void Player::Update(double delta_time){
	SceneNode *rotor = children[0];

	rotor->Rotate(glm::angleAxis((float) (glm::radians(90.0) * delta_time), glm::vec3(0.0, 1.0, 0.0)));
	//std::cout << "hello world!!!" << std::endl;
}


            
} // namespace game
