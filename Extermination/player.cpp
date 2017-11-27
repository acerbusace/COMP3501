#include "player.h"

namespace game {

Player::Player(std::string name, Resource *geometry, Resource *material, Resource *texture) : SceneNode(name, geometry, material, texture) {
	first_person_ = false;
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


bool Player::get_first_person()
{
	return first_person_;
}

void Player::toggle_first_person()
{
	first_person_ = (first_person_ == true) ? false : true;
}




            
} // namespace game
