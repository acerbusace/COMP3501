//#ifndef MISC_H_
//#define MISC_H_

#include <exception>
#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>

#include "scene_graph.h"
#include "laser.h"
#include "bin/path_config.h"
#include "resource_manager.h"

namespace game {
	Resource *getResource(ResourceManager *resman, std::string name) {
		Resource *res = resman->GetResource(name);
		if (!res) {
			throw(std::string("Could not find resource \"") + name + std::string("\""));
		}
		return res;
	}

	Laser *createLaserInstance(ResourceManager *resman) {
		Resource *geom = getResource(resman, "LaserMesh");
		Resource *mat = getResource(resman, "ShinyTextureMaterial");
		Resource *tex = resman->GetResource("window");

		Laser *lsr = new Laser("Laser", geom, mat, tex);
		lsr->Scale(glm::vec3(0.15, 0.15, 1.0));
		return lsr;
	}
}

//#endif