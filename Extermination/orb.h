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

    // Abstraction of the Orb enemy
    class Orb : public SceneNode {

        public:
            // Create Orb from given resources
			Orb(std::string name, Resource *geometry, Resource *material, Resource *texture = 0);

            // Destructor
            ~Orb();
            
            // Update geometry 
            void Update(double);

			float getFireSpeed();
			float getLaserSpeed();
			float getLaserDamage();

			bool done();
			bool shoot();
			void fireCoolDown();
			float fireError();
			bool takeDamage(float);

			void setFireSpeed(float);
			void setFireError(float);
			void setLaserSpeed(float);
			void setLaserDamage(float);
			void setTextureAlt(Resource*);
            
        private:
			float fire_error_;
			float laser_speed_;
			float fire_speed_;
			float fire_cooldown_;
			float laser_damage_;
			float health_;
            GLuint textureAct_; 
            GLuint textureAlt_; 
    };

} // namespace game

#endif
