#include <exception>
#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include <iostream>

#include "scene_graph.h"
#include "laser.h"
#include "bin/path_config.h"
#include "resource_manager.h"

#include "misc.h"

namespace game {
	Resource *getResource(ResourceManager *resman, std::string name) {
		Resource *res = resman->GetResource(name);
		if (!res) {
			throw(std::string("Could not find resource \"") + name + std::string("\""));
		}
		return res;
	}

	Laser *createLaserInstance(ResourceManager *resman) {
		Resource *geom = getResource(resman, "CubeMesh");
		Resource *mat = getResource(resman, "ShinyTextureMaterial");
		Resource *tex = resman->GetResource("window");

		Laser *lsr = new Laser("Laser", geom, mat, tex);
		lsr->Scale(glm::vec3(0.15, 0.15, 1.0));
		return lsr;
	}

	Bomb *createBombInstance(ResourceManager *resman) {
		Resource *geom = getResource(resman, "SphereMesh");
		Resource *mat = getResource(resman, "ShinyBlueMaterial");
		//Resource *tex = resman->GetResource("window");

		//Laser *lsr = new Laser("Laser", geom, mat, tex);
		Bomb *bmb = new Bomb("Bomb", geom, mat);
		bmb->Scale(glm::vec3(0.35, 0.35, 0.35));
		return bmb;
	}

	SceneNode *createParticleInstance(ResourceManager *resman, std::string object_name, std::string material_name, std::string texture_name){
		Resource *geom = getResource(resman, object_name);
		Resource *mat = getResource(resman, material_name);
		Resource *tex = resman->GetResource(texture_name);

		SceneNode *scn = new SceneNode("Particle", geom, mat, tex);
		return scn;
	}

	bool collision(Bomb* bomb, SceneNode* node) {
		float dist = glm::length(bomb->getPos() - node->getPos());
		if (dist < bomb->GetRadius() + node->GetRadius()) {
			//std::cout << "collision!!!" << std::endl;
			return true;
		}
		return false;
	}

	bool collision(SceneNode* particle, SceneNode* node) {
		float dist = glm::length(particle->getPos() - node->getPos());
		if (dist < particle->GetRad() + node->GetRadius()) {
			//std::cout << "collision!!!" << std::endl;
			return true;
		}
		return false;
	}

	void printVec3(glm::vec3 v) {
		std::cout << "x: " << v.x << ", " << v.y << ", " << v.z << std::endl;
	}

} // namespace game
