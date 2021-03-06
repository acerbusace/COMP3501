#ifndef LASER_H_
#define LASER_H_

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

namespace game {

    // Abstraction of a laser
    class Laser : public SceneNode {

        public:
            // Create laser from given resources
            Laser(std::string name, Resource *geometry, Resource *material, Resource *texture = 0);

            // Destructor
            ~Laser();
            
            void Update(double);
			float GetDamage();
			bool done();

			void SetInitPos(glm::vec3);
			void SetSpeed(float);
			void SetDamage(float);
			std::vector<glm::vec3> GetLaserPoints();
            
        private:
			glm::mat4 getTransf();
			glm::vec3 init_pos_;
			float speed_;
			float dist_;
			float damage_;
    };

} // namespace game

#endif
