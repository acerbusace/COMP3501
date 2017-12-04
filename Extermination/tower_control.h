#ifndef TOWER_CONTROL_H_
#define TOWER_CONTROL_H_

#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>
#include <vector>

#include "resource.h"
#include "scene_node.h"
#include "orb.h"
#include "tower.h"
#include "camera.h"
#include "laser.h"
#include "player.h"

namespace game {

    class TowerControl {

        public:
            // Create asteroid from given resources
			TowerControl(ResourceManager*);
			void init();

            // Destructor
            ~TowerControl();

            // Update geometry configuration
            void update(double, Player*);
			// Update geometry configuration
			void draw(Camera*);
            
        private:
			ResourceManager *resman_;

			std::vector<Tower*> towers_;
			std::vector<Orb*> orbs_;
			std::vector<Laser*> lasers_;

			Tower *createTowerInstance(glm::vec3);
			void shoot(Orb*, glm::vec3);

			bool test;
    };

} // namespace game

#endif
