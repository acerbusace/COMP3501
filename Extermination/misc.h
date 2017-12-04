#ifndef MISC_H_
#define MISC_H_

#include "laser.h"
#include "bomb.h"
#include "missile.h"
#include "resource_manager.h"

namespace game {

	Resource *getResource(ResourceManager *resman, std::string name);

	Laser *createLaserInstance(ResourceManager *resman);
	Bomb *createBombInstance(ResourceManager *resman);
	Missile *createMissileInstance(ResourceManager *resman);
	SceneNode *createParticleInstance(ResourceManager *resman, std::string object_name, std::string material_name, std::string texture_name = std::string(""));
}

#endif