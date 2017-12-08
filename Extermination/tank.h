#ifndef TANK_H_
#define TANK_H_

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

    // Abstraction of the Tank enemy
    class Tank : public SceneNode {

        public:
            // Create Tank from given resources
			Tank(std::string name, Resource *geometry, Resource *material, Resource *texture = 0);

            // Destructor
            ~Tank();
            
            // Update geometry configuration
            void Update(double);

			float getFireSpeed();
			float getBombSpeed();
			float getBombTimer();
			float GetBombDamage();
			float GetBodyDamage();
			float getMoveError();
			float GetRadius();

			bool move();
			float moveError();
			bool shoot();
			void fireCoolDown();
			bool takeDamage(float);

			void setFireSpeed(float);
			void setBombSpeed(float);
			void setBombTimer(float);
			void SetBombDamage(float);
			void setMove(bool);
			void setMoveError(float);
			void setDestination(glm::vec2);

            
        private:
			float bomb_timer_;
			float bomb_speed_;
			float fire_speed_;
			float fire_cooldown_;
			float move_error_;
			float move_speed_;
			float bomb_damage_;
			float body_damage_;
			float health_;
			glm::vec3 dest_;
			glm::vec3 velocity;
			bool move_;
    };

} // namespace game

#endif
