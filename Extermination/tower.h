#ifndef TOWER_H_
#define TOWER_H_

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
    class Tower : public SceneNode {

        public:
            // Create laser from given resources
			Tower(std::string name, Resource *geometry, Resource *material, Resource *texture = 0);

            // Destructor
            ~Tower();
            
            // Update geometry configuration
            void Update(double);
            
        private:
    };

} // namespace game

#endif
