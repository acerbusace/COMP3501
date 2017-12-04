#ifndef MISC_H_
#define MISC_H_

#include "laser.h"
#include "bomb.h"
#include "orb.h"
#include "missile.h"
#include "resource_manager.h"

namespace game {

	Resource *getResource(ResourceManager *resman, std::string name);

	Laser *createLaserInstance(ResourceManager *resman);
	Bomb *createBombInstance(ResourceManager *resman);
	Missile *createMissileInstance(ResourceManager *resman);
	SceneNode *createParticleInstance(ResourceManager *resman, std::string object_name, std::string material_name, std::string texture_name = std::string(""));

	bool collision(Bomb*, SceneNode*);
	bool collision(SceneNode*, SceneNode*);
	bool collision(Laser*, SceneNode*);

	void printVec3(glm::vec3);

	static const float SPEHRE_PARTICLE_SPEED = 0.283197;
}

#endif