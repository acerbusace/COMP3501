#include "player.h"

namespace game {

Player::Player(const std::string name, const Resource *geometry, const Resource *material) : SceneNode(name, geometry, material) {
}


Player::~Player(){
}


            
} // namespace game
