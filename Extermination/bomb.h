#ifndef BOMB_H_
#define BOMB_H_

#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>

#include "resource.h"
#include "scene_node.h"

namespace game {

    // Abstraction of a bomb
    class Bomb : public SceneNode {

        public:
            // Create bomb from given resources
            Bomb(std::string name, Resource *geometry, Resource *material, Resource *texture = 0);

            // Destructor
            ~Bomb();
            
            void Update(double);
			float GetDamage();
			float GetExpRadius();
			bool Explode();

			void SetSpeed(float);
			void SetTimer(float);
			void SetDamage(float);
			void SetExpRadius(float);
            
        private:
			bool explode_;
			float timer_;
			float speed_;
			float radius_;
			float damage_;
    };

} // namespace game

#endif
