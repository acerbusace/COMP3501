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

    // Abstraction of an laser
    class Bomb : public SceneNode {

        public:
            // Create laser from given resources
            Bomb(std::string name, Resource *geometry, Resource *material, Resource *texture = 0);

            // Destructor
            ~Bomb();
            
            // Update geometry configuration
            void Update(double);
			void SetSpeed(float);
			void SetInitPos(glm::vec3);
			void SetTimer(float);
            
        private:
			glm::mat4 getTransf();
			glm::vec3 init_pos_;
			float timer_;
			float speed_;
    };

} // namespace game

#endif
