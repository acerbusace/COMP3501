#include <iostream>
#include <time.h>
#include <sstream>

#include "game.h"
#include "bin/path_config.h"

namespace game {

// Some configuration constants
// They are written here as global variables, but ideally they should be loaded from a configuration file

// Main window settings
const std::string window_title_g = "Demo";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
const bool window_full_screen_g = false;
const std::string SHINY_BLUE_MATERIAL = "ShinyBlueMaterial";
const std::string SHINY_TEXTURE_MATERIAL = "ShinyTextureMaterial";

// Viewport and camera settings
float camera_near_clip_distance_g = 0.01;
float camera_far_clip_distance_g = 1000.0;
float camera_fov_g = 20.0; // Field-of-view of camera
const glm::vec3 viewport_background_color_g(1.0, 1.0, 1.0);
glm::vec3 camera_position_g(0.0, 0.0, 10.0);
glm::vec3 camera_look_at_g(0.0, 0.0, 0.0);
glm::vec3 camera_up_g(0.0, 1.0, 0.0);

// Materials 
const std::string material_directory_g = MATERIAL_DIRECTORY;


Game::Game(void){

    // Don't do work in the constructor, leave it for the Init() function
}


void Game::Init(void){

    // Run all initialization steps
    InitWindow();
    InitView();
    InitEventHandlers();

    // Set variables
    animating_ = true;
	material_ = true;
	last_time = 0;
}

       
void Game::InitWindow(void){

    // Initialize the window management library (GLFW)
    if (!glfwInit()){
        throw(GameException(std::string("Could not initialize the GLFW library")));
    }

    // Create a window and its OpenGL context
    if (window_full_screen_g){
        window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g.c_str(), glfwGetPrimaryMonitor(), NULL);
    } else {
        window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g.c_str(), NULL, NULL);
    }
    if (!window_){
        glfwTerminate();
        throw(GameException(std::string("Could not create window")));
    }

    // Make the window's context the current one
    glfwMakeContextCurrent(window_);

    // Initialize the GLEW library to access OpenGL extensions
    // Need to do it after initializing an OpenGL context
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK){
        throw(GameException(std::string("Could not initialize the GLEW library: ")+std::string((const char *) glewGetErrorString(err))));
    }
}


void Game::InitView(void){

    // Set up z-buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Set viewport
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    glViewport(0, 0, width, height);

    // Set up camera
    // Set current view
    camera_.SetView(camera_position_g, camera_look_at_g, camera_up_g);
    // Set projection
    camera_.SetProjection(camera_fov_g, camera_near_clip_distance_g, camera_far_clip_distance_g, width, height);
}


void Game::InitEventHandlers(void){

    // Set event callbacks
    glfwSetKeyCallback(window_, KeyCallback);
    glfwSetFramebufferSizeCallback(window_, ResizeCallback);

    // Set pointer to game object, so that callbacks can access it
    glfwSetWindowUserPointer(window_, (void *) this);
}


void Game::SetupResources(void){

    // Create a torus
    resman_.CreateSphere("SphereMesh");
    resman_.CreateTorus("TorusMesh");
    resman_.CreateCube("CubeMesh");
    resman_.CreateCylinder("CylinderMesh");

	std::string filename;
    // Load material to be applied to torus
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/shiny_blue");
    resman_.LoadResource(Material, SHINY_BLUE_MATERIAL, filename.c_str());

	// Load material to be applied to sphere
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/shiny_texture");
	resman_.LoadResource(Material, SHINY_TEXTURE_MATERIAL, filename.c_str());

	// Load window texture
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/window.jpg");
	resman_.LoadResource(Texture, "Window", filename.c_str());

	// Load metal texture
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/metal.jpg");
	resman_.LoadResource(Texture, "Metal", filename.c_str());

	// Load a cube from a file
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/SHIP.obj");
	resman_.LoadResource(Mesh, "PlayerMesh", filename.c_str());
}


void Game::SetupScene(void){

    // Set background color for the scene
    scene_.SetBackgroundColor(viewport_background_color_g);

    // Create an instance of the torus mesh
    game::SceneNode *torus = CreateInstance("TorusInstance1", "TorusMesh", SHINY_BLUE_MATERIAL);
    // Scale the instance
    torus->Scale(glm::vec3(0.75, 0.75, 0.75));
    torus->Translate(glm::vec3(-1.0, 0, 0));

	//game::SceneNode *player = CreateInstance("PlayerInstance", "TorusMesh", SHINY_BLUE_MATERIAL);
	CreatePlayerInstance("PlayerInstance", "PlayerMesh", SHINY_BLUE_MATERIAL);


    // Create an helicopter instance

	// upper body
    game::SceneNode *upper_body = CreateInstance("CubeInstance1", "CubeMesh", SHINY_TEXTURE_MATERIAL, "Window");
    upper_body->Scale(glm::vec3(0.35, 0.35, 1.25));
	upper_body->Rotate(glm::angleAxis((float) glm::radians(1.0), glm::vec3(15.0, 0.0, 0.0)));
    upper_body->Translate(glm::vec3(1.0, 0, 0));

	// lower body
    game::SceneNode *lower_body = CreateInstance("CubeInstance2", "CubeMesh", SHINY_TEXTURE_MATERIAL, "Window");
    lower_body->Scale(glm::vec3(0.35, 0.35, 1.5));
    lower_body->Translate(glm::vec3(0.0, -0.35, 0.125));

	// upper joint (cylinder connecting body and upper rotor)
    game::SceneNode *upper_joint = CreateInstance("CylinderInstance1", "CylinderMesh", SHINY_TEXTURE_MATERIAL, "Metal");
    upper_joint->Scale(glm::vec3(0.45, 0.10, 0.45));
    upper_joint->Translate(glm::vec3(0.0, 0.225, 0));

	// upper rotor
    game::SceneNode *upper_rotor = CreateInstance("CylinderInstance2", "CylinderMesh", SHINY_TEXTURE_MATERIAL, "Metal");
    upper_rotor->Scale(glm::vec3(0.05, 1.0, 0.05));
	upper_rotor->Rotate(glm::angleAxis((float) glm::radians(1.0), glm::vec3(0.0, 0.0, 115.0)));

	// back joint (cylinder connecting body and back rotor)
    game::SceneNode *back_joint = CreateInstance("CylinderInstance3", "CylinderMesh", SHINY_TEXTURE_MATERIAL, "Metal");
    back_joint->Scale(glm::vec3(0.35, 0.75, 0.35));
	back_joint->Rotate(glm::angleAxis((float) glm::radians(1.0), glm::vec3(115.0, 0.0, 0.0)));
    back_joint->Translate(glm::vec3(0.0, 0.0, -1.0));

	// back rotor
    game::SceneNode *back_rotor = CreateInstance("CylinderInstance4", "CylinderMesh", SHINY_TEXTURE_MATERIAL, "Metal");
    back_rotor->Scale(glm::vec3(0.05, 0.75, 0.05));
	back_rotor->Rotate(glm::angleAxis((float) glm::radians(1.0), glm::vec3(90.0, 0.0, 0.0)));
    back_rotor->Translate(glm::vec3(0.10, -0.25, 0.0));

	// creates helicopter hierarchy 
	upper_body->addChild(lower_body);
	upper_body->addChild(upper_joint);
	upper_joint->addChild(upper_rotor);
	upper_body->addChild(back_joint);
	back_joint->addChild(back_rotor);

	CreateLand(glm::vec3(10, 1, 10), glm::vec3(0.0, -0.05, 0.0), glm::vec3(10.0, 0.10, 10.0));
}


void Game::MainLoop(void){

    // Loop while the user did not close the window
	animating_ = true;
    while (!glfwWindowShouldClose(window_)){
        // Animate the scene
        if (animating_){
            double current_time = glfwGetTime();
			double delta_time = current_time - last_time;
            last_time = current_time;

			SceneNode *node;
			node = scene_.GetNode("PlayerInstance");

			Movement(node, delta_time);



			//std::cout << "time: " << current_time << " -> " << current_time - last_time << std::endl;
            //scene_.Update();
			std::cout << glm::to_string(camera_.GetForward());

			glm::quat rotation;

            // Animate the torus and helicopter
			rotation = glm::angleAxis((float) glm::radians(100.0) * (float) delta_time, glm::vec3(0.0, 1.0, 0.0));

            node = scene_.GetNode("TorusInstance1");
            node->Rotate(rotation);

            node = scene_.GetNode("CubeInstance1");
            node->Rotate(rotation);

			// animate top and back rotor
			rotation = glm::angleAxis((float) glm::radians(100.0) * (float) delta_time, glm::vec3(2.0, 0.0, 0.0));

            node = scene_.GetNode("CylinderInstance2");
            node->Rotate(rotation);
            node = scene_.GetNode("CylinderInstance4");
            node->Rotate(rotation);
        }

        // Draw the scene
        scene_.Draw(&camera_);

        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);

        // Update other events like input handling
        glfwPollEvents();
    }
}


void Game::Movement(SceneNode* node, double delta_time) {
	float rot_factor = glm::radians(40.0) * delta_time;
	float roll_factor = glm::radians(2000.0) * delta_time;
	float trans_factor = 5.0 * delta_time;
	float camera_factor = 10.0;

	//Move Forward
	if (key_[GLFW_KEY_UP] == GLFW_PRESS || key_[GLFW_KEY_UP] == GLFW_REPEAT) {
		node->Translate(camera_.GetForward() * trans_factor);
		camera_.Translate(camera_.GetForward() * trans_factor);
	}
	//Move Backward
	if (key_[GLFW_KEY_DOWN] == GLFW_PRESS || key_[GLFW_KEY_DOWN] == GLFW_REPEAT) {
		node->Translate(camera_.GetForward() * -trans_factor);
		camera_.Translate(camera_.GetForward() * -trans_factor);
	}
	//Move Left
	if (key_[GLFW_KEY_LEFT] == GLFW_PRESS || key_[GLFW_KEY_LEFT] == GLFW_REPEAT) {
		node->Translate(camera_.GetSide() * -trans_factor);
		camera_.Translate(camera_.GetSide() * -trans_factor);
	}
	//Move Right
	if (key_[GLFW_KEY_RIGHT] == GLFW_PRESS || key_[GLFW_KEY_RIGHT] == GLFW_REPEAT) {
		node->Translate(camera_.GetSide() * trans_factor);
		camera_.Translate(camera_.GetSide() * trans_factor);
	}
	//Move Upwards
	if (key_[GLFW_KEY_W] == GLFW_PRESS || key_[GLFW_KEY_W] == GLFW_REPEAT) {
		node->Translate(camera_.GetUp() * trans_factor);
		camera_.Translate(camera_.GetUp() * trans_factor);
	}
	//Move Downwards
	if (key_[GLFW_KEY_E] == GLFW_PRESS || key_[GLFW_KEY_E] == GLFW_REPEAT) {
		node->Translate(camera_.GetUp() * -trans_factor);
		camera_.Translate(camera_.GetUp() * -trans_factor);
	}
	//Roll Left
	if (key_[GLFW_KEY_1] == GLFW_PRESS || key_[GLFW_KEY_1] == GLFW_REPEAT) {
		node->Rotate(glm::angleAxis((float)glm::radians(1.0), glm::vec3(0.0, 0.0, roll_factor)));
		camera_.Translate(camera_.GetForward() * camera_factor);
		camera_.Rotate(glm::angleAxis((float)glm::radians(1.0), glm::vec3(0.0, 0.0, roll_factor)));
		camera_.Translate(camera_.GetForward() * -camera_factor);
	}
	//Roll Right
	if (key_[GLFW_KEY_2] == GLFW_PRESS || key_[GLFW_KEY_2] == GLFW_REPEAT) {
		node->Rotate(glm::angleAxis((float)glm::radians(1.0), glm::vec3(0.0, 0.0, -roll_factor)));
		camera_.Translate(camera_.GetForward() * camera_factor);
		camera_.Rotate(glm::angleAxis((float)glm::radians(1.0), glm::vec3(0.0, 0.0, -roll_factor)));
		camera_.Translate(camera_.GetForward() * -camera_factor);
	}
	//Roll Forward
	if (key_[GLFW_KEY_3] == GLFW_PRESS || key_[GLFW_KEY_3] == GLFW_REPEAT) {
		node->Rotate(glm::angleAxis((float)glm::radians(1.0), glm::vec3(-roll_factor, 0.0, 0.0)));
		camera_.Translate(camera_.GetForward() * camera_factor);
		camera_.Rotate(glm::angleAxis((float)glm::radians(1.0), glm::vec3(-roll_factor, 0.0, 0.0)));
		camera_.Translate(camera_.GetForward() * -camera_factor);
	}
	//Roll Back
	if (key_[GLFW_KEY_4] == GLFW_PRESS || key_[GLFW_KEY_4] == GLFW_REPEAT) {
		node->Rotate(glm::angleAxis((float)glm::radians(1.0), glm::vec3(roll_factor, 0.0, 0.0)));
		camera_.Translate(camera_.GetForward() * camera_factor);
		camera_.Rotate(glm::angleAxis((float)glm::radians(1.0), glm::vec3(roll_factor, 0.0, 0.0)));
		camera_.Translate(camera_.GetForward() * -camera_factor);
	}
	//Turn Left
	if (key_[GLFW_KEY_5] == GLFW_PRESS || key_[GLFW_KEY_5] == GLFW_REPEAT) {
		node->Rotate(glm::angleAxis((float)glm::radians(1.0), glm::vec3(0.0, roll_factor, 0.0)));
		camera_.Translate(camera_.GetForward() * camera_factor);
		camera_.Rotate(glm::angleAxis((float)glm::radians(1.0), glm::vec3(0.0, roll_factor, 0.0)));
		camera_.Translate(camera_.GetForward() * -camera_factor);
	}
	//Turn Right
	if (key_[GLFW_KEY_6] == GLFW_PRESS || key_[GLFW_KEY_6] == GLFW_REPEAT) {
		node->Rotate(glm::angleAxis((float)glm::radians(1.0), glm::vec3(0.0, -roll_factor, 0.0)));
		camera_.Translate(camera_.GetForward() * camera_factor);
		camera_.Rotate(glm::angleAxis((float)glm::radians(1.0), glm::vec3(0.0, -roll_factor, 0.0)));
		camera_.Translate(camera_.GetForward() * -camera_factor);
	}
}


void Game::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){

	// Get user data with a pointer to the game class
	void* ptr = glfwGetWindowUserPointer(window);
	Game *game = (Game *)ptr;

	game->key_[key] = action;

	// Quit game if 'q' is pressed
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	// Stop animation if space bar is pressed
	if (key == GLFW_KEY_P && action == GLFW_PRESS){
	    game->animating_ = (game->animating_ == true) ? false : true;
	}

	double delta = glfwGetTime() - game->last_time;

	SceneNode *node;
	node = game->scene_.GetNode("CubeInstance1");

	// View control
	float rot_factor(glm::pi<float>() / 9 * delta);
	float roll_factor(glm::pi<float>() / 2 * delta);
	float trans_factor = 10.0 * delta;

    if (key == GLFW_KEY_UP){
        //game->camera_.Pitch(rot_factor);
    }
    if (key == GLFW_KEY_DOWN){
        //game->camera_.Pitch(-rot_factor);
    }
    if (key == GLFW_KEY_LEFT){
        //game->camera_.Yaw(rot_factor);
    }
    if (key == GLFW_KEY_RIGHT){
        //game->camera_.Yaw(-rot_factor);
    }
    /*if (key == GLFW_KEY_S){
        game->camera_.Roll(-rot_factor);
    }
    if (key == GLFW_KEY_X){
        game->camera_.Roll(rot_factor);
    }
    if (key == GLFW_KEY_A){
		game->camera_.Translate(glm::vec3(0, 0, -trans_factor));
    }
    if (key == GLFW_KEY_Z){
		game->camera_.Translate(glm::vec3(0, 0, trans_factor));
    }
    if (key == GLFW_KEY_J){
		game->camera_.Translate(glm::vec3(-trans_factor, 0, 0));
    }
    if (key == GLFW_KEY_L){
		game->camera_.Translate(glm::vec3(-trans_factor, 0, 0));
    }
    if (key == GLFW_KEY_SPACE){
		game->camera_.Translate(glm::vec3(0, trans_factor, 0));
    }
    if (key == GLFW_KEY_LEFT_CONTROL){
		game->camera_.Translate(glm::vec3(0, -trans_factor, 0));
    }*/

	if (key == GLFW_KEY_1) {
		//node->Rotate(glm::angleAxis((float)glm::radians(1.0), glm::vec3(0.0, 0.0, -roll_factor)));
	}
	if (key == GLFW_KEY_2) {
		//node->Rotate(glm::angleAxis((float)glm::radians(1.0), glm::vec3(0.0, 0.0, roll_factor)));
	}
	if (key == GLFW_KEY_3) {
		//node->Rotate(glm::angleAxis((float)glm::radians(1.0), glm::vec3(-roll_factor, 0, 0)));
	}
	if (key == GLFW_KEY_4) {
		//node->Rotate(glm::angleAxis((float)glm::radians(1.0), glm::vec3(roll_factor, 0, 0)));
	}
	if (key == GLFW_KEY_5) {
		//node->Rotate(glm::angleAxis((float)glm::radians(1.0), glm::vec3(0, -roll_factor, 0)));
	}
	if (key == GLFW_KEY_6) {
		//node->Rotate(glm::angleAxis((float)glm::radians(1.0), glm::vec3(0, roll_factor, 0)));
	}
	if (key == GLFW_KEY_V) {
		//game->camera_.SetView(camera_position_g, camera_look_at_g, camera_up_g);
	}
}


void Game::ResizeCallback(GLFWwindow* window, int width, int height){

    // Set up viewport and camera projection based on new window size
    glViewport(0, 0, width, height);
    void* ptr = glfwGetWindowUserPointer(window);
    Game *game = (Game *) ptr;
    game->camera_.SetProjection(camera_fov_g, camera_near_clip_distance_g, camera_far_clip_distance_g, width, height);
}


Game::~Game(){
    
    glfwTerminate();
}


Player *Game::CreatePlayerInstance(std::string entity_name, std::string object_name, std::string material_name){

    // Get resources
    Resource *geom = resman_.GetResource(object_name);
    if (!geom){
        throw(GameException(std::string("Could not find resource \"")+object_name+std::string("\"")));
    }

    Resource *mat = resman_.GetResource(material_name);
    if (!mat){
        throw(GameException(std::string("Could not find resource \"")+material_name+std::string("\"")));
    }

    // Create asteroid instance
    Player *player = new Player(entity_name, geom, mat);
    scene_.AddNode(player);
    return player;
}


void Game::CreateLand(glm::vec3 size, glm::vec3 pos, glm::vec3 scale){
	game::SceneNode *node;
    // Create a number of asteroid instances
	for (int x = 0; x < size.x; ++x) {
		for (int y = 0; y < size.y; ++y) {
			for (int z = 0; z < size.z; ++z) {
				node = CreateInstance("Land", "CubeMesh", SHINY_TEXTURE_MATERIAL, "Window");
				node->Scale(scale);
				node->Translate(pos + glm::vec3(scale.x*x, scale.y*y, scale.z*z));
			}
		}
	}
}


SceneNode *Game::CreateInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name){

    Resource *geom = resman_.GetResource(object_name);
    if (!geom){
        throw(GameException(std::string("Could not find resource \"")+object_name+std::string("\"")));
    }

    Resource *mat = resman_.GetResource(material_name);
    if (!mat){
        throw(GameException(std::string("Could not find resource \"")+material_name+std::string("\"")));
    }

    Resource *tex = resman_.GetResource(texture_name);

    SceneNode *scn = scene_.CreateNode(entity_name, geom, mat, tex);
    return scn;
}

} // namespace game
