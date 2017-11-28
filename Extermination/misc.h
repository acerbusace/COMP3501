#ifndef MISC_H_
#define MISC_H_

#include "laser.h"
#include "resource_manager.h"

namespace game {
	Resource *getResource(ResourceManager *resman, std::string name);

	Laser *createLaserInstance(ResourceManager *resman);
}

#endif