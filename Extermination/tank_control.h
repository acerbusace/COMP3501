#ifndef TANK_CONTROL_H_
#define TANK_CONTROL_H_

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
#include "tank.h"
#include "camera.h"
#include "bomb.h"
#include "player.h"

namespace game {

    class TankControl {

        public:
            // Create asteroid from given resources
			TankControl(ResourceManager*);
			void init(glm::vec3);

            // Destructor
            ~TankControl();

            // Update geometry configuration
            //void update(double, glm::vec3 player_pos);
            void update(double, Player*);
			// Update geometry configuration
			void draw(Camera*);
            
        private:
			ResourceManager *resman_;

			std::vector<Tank*> tanks_;
			//std::vector<Laser*> lasers_;
			std::vector<Bomb*> bombs_;
			std::vector<SceneNode*> bomb_particles_;
			std::vector<SceneNode*> mines_;
			std::vector<float> mines_reset_;
			std::vector<float> mines_timer_;

			Tank *createTankInstance(glm::vec3);
			SceneNode *createMineInstance(glm::vec3);
			void shoot(Tank*, glm::vec3);
			void move(Tank*, glm::vec3);

			bool test;
    };

} // namespace game

#endif
