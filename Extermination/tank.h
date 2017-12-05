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

    // Abstraction of an laser
    class Tank : public SceneNode {

        public:
            // Create laser from given resources
			Tank(std::string name, Resource *geometry, Resource *material, Resource *texture = 0);

            // Destructor
            ~Tank();
            
            // Update geometry configuration
            void Update(double);
			float moveError();
			float getBombSpeed();
			float getFireSpeed();
			float getMoveError();
			float getBombTimer();
			bool shoot();
			void fireCoolDown();
			void setBombSpeed(float);
			void setFireSpeed(float);
			void setBombTimer(float);
			void setMoveError(float);
			void setMove(bool);
			bool move();
			void setDestination(glm::vec2);
			float GetBombDamage();
			float GetBodyDamage();
			void SetBombDamage(float);
			bool takeDamage(float);
			float GetRadius();
            
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
