#ifndef GAME_H_
#define GAME_H_

#include <exception>
#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>

#include "glm/ext.hpp"
#include "scene_graph.h"
#include "resource_manager.h"
#include "camera.h"
#include "player.h"
#include "laser.h"
#include "bomb.h"
#include "tower_control.h"

namespace game {

    // Exception type for the game
    class GameException: public std::exception
    {
        private:
            std::string message_;
        public:
            GameException(std::string message) : message_(message) {};
            virtual const char* what() const throw() { return message_.c_str(); };
            virtual ~GameException() throw() {};
    };

    // Game application
    class Game {

        public:
            // Constructor and destructor
            Game(void);
            ~Game();
            // Call Init() before calling any other method
            void Init(void); 
            // Set up resources for the game
            void SetupResources(void);
            // Set up initial scene
            void SetupScene(void);
            // Run the game: keep the application active
            void MainLoop(void);

        private:
            // GLFW window
            GLFWwindow *window_;

            // Scene graph containing all nodes to render
            SceneGraph *scene_;

            // Resources available to the game
            ResourceManager *resman_;

            // Camera abstraction
            Camera camera_;

			// Tower Control
			TowerControl *tower_control_;

            // Flag to turn animation on/off
            bool animating_;

			bool material_;

			std::map<int, int> key_;

			double last_time;

            // Methods to initialize the game
            void InitWindow(void);
            void InitView(void);
            void InitEventHandlers(void);
 
            // Methods to handle events
			void input(SceneNode * node, double delta_time);
			void update(SceneNode * node, double delta_time);
            static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
            static void ResizeCallback(GLFWwindow* window, int width, int height);

            // Create player
            Player *CreatePlayerInstance(std::string entity_name, std::string object_name, std::string material_name);

			// Create land
			void Game::CreateLand(glm::vec3 size, glm::vec3 pos = glm::vec3(0.0, -0.5, 0.0), glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0));

            // Create an instance of an object stored in the resource manager
            SceneNode *CreateInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name = std::string(""));

			// Create an instance of a laser
			Laser *CreateLaserInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name = std::string(""));

			// Create an instance of a bomb
			Bomb *CreateBombInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name = std::string(""));


    }; // class Game

} // namespace game

#endif // GAME_H_
