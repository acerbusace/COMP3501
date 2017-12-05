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

class Laser;
class Bomb;
class Missile;
class Camera;
class ResourceManager;

    // Abstraction of an asteroid
    class Player : public SceneNode {

        public:
            // Create asteroid from given resources
            Player(std::string name = std::string(""), Resource *geometry = NULL, Resource *material = NULL, Resource *texture = NULL);

            // Destructor
            ~Player();
            
			void Draw(Camera *camera, glm::vec3);
			void Update(double delta_time);

			bool get_first_person();
			void toggle_first_person();
			bool takeDamage(float);

			void addMissile(Camera*);
			void addBomb();
			void addLaser(Camera*);
			void addBombParticle(glm::vec3);
			void setResman(ResourceManager*);

			std::vector<Laser*>* getLasers();
			std::vector<Bomb*>* getBombs();
			std::vector<Missile*>* getMissiles();
			std::vector<SceneNode*>* getBombParticles();

        private:
			bool first_person_;
			float health_;

			ResourceManager* resman_;

			std::vector<Laser*> lasers_;
			std::vector<Bomb*> bombs_;
			std::vector<Missile*> missiles_;
			std::vector<SceneNode*> bomb_particles_;
    }; // class Player

} // namespace game

#endif // PLAYER_H_
