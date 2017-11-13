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
            Player(const std::string name, const Resource *geometry, const Resource *material);

            // Destructor
            ~Player();
            
            
        private:
    }; // class Player

} // namespace game

#endif // PLAYER_H_
