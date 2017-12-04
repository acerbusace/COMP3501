#include <stdexcept>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "tower.h"

namespace game {

Tower::Tower(std::string name, Resource *geometry, Resource *material, Resource *texture) : SceneNode(name, geometry, material, texture) {
	
}


Tower::~Tower(){
}

void Tower::Update(double delta_time){

}
            
} // namespace game
