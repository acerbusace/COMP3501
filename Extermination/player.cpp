#include "player.h"

namespace game {

Player::Player(std::string name, Resource *geometry, Resource *material, Resource *texture) : SceneNode(name, geometry, material, texture) {
}


Player::~Player(){
}


            
} // namespace game
