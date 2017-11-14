#include "asteroid.h"

namespace game {

Asteroid::Asteroid(std::string name, Resource *geometry, Resource *material) : SceneNode(name, geometry, material) {
}


Asteroid::~Asteroid(){
}


glm::quat Asteroid::GetAngM(void) const {

    return angm_;
}


void Asteroid::SetAngM(glm::quat angm){

    angm_ = angm;
}


void Asteroid::Update(void){

    Rotate(angm_);
}
            
} // namespace game
