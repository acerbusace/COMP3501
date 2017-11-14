#ifndef ORB_H_
#define ORB_H_

#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>

#include "resource_manager.h"
#include "scene_node.h"

namespace game {

    // Abstraction of an laser
    class Orb : public SceneNode {

        public:
            // Create laser from given resources
			Orb(std::string name, Resource *geometry, Resource *material, Resource *texture = 0);

            // Destructor
            ~Orb();
            
            // Update geometry configuration
            void Update(double);
			float fireError();
			float getLaserSpeed();
			float getFireSpeed();
			bool shoot();
			void fireCoolDown();
			void Orb::setLaserSpeed(float);
			void Orb::setFireSpeed(float);
			void Orb::setFireError(float);
            
        private:
			float fire_error_;
			float laser_speed_;
			float fire_speed_;
			float fire_cooldown_;
    };

} // namespace game

#endif
