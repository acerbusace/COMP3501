#ifndef ASTEROID_H_
#define ASTEROID_H_

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

    // Abstraction of an asteroid
    class Player : public SceneNode {

        public:
            // Create asteroid from given resources
            Player(std::string name = std::string(""), Resource *geometry = NULL, Resource *material = NULL, Resource *texture = NULL);

            // Destructor
            ~Player();
            
			void Draw(Camera *camera);
			void Update(double delta_time);

			bool get_first_person();
			void toggle_first_person();
			bool takeDamage(float);

        private:
			bool first_person_;
			float health_;
    }; // class Player

} // namespace game

#endif // PLAYER_H_
