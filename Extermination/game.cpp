#include <iostream>
#include <time.h>
#include <sstream>

#include "game.h"
#include "misc.h"
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

	resman_ = new ResourceManager();
	scene_ = new SceneGraph();
	tower_control_ = new TowerControl(resman_);
	tank_control_ = new TankControl(resman_);
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
	resman_->CreateSphere("SphereMesh");
	resman_->CreateTorus("TorusMesh");
	resman_->CreateCube("CubeMesh");
	resman_->CreateCylinder("CylinderMesh");

	std::string filename;

	// Load Shiny Blue Material
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/shiny_blue");
	resman_->LoadResource(Material, SHINY_BLUE_MATERIAL, filename.c_str());

	// Load Shiny Texture Material
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/shiny_texture");
	resman_->LoadResource(Material, SHINY_TEXTURE_MATERIAL, filename.c_str());

	filename = std::string(MATERIAL_DIRECTORY) + std::string("/texture");
	resman_->LoadResource(Material, "BrightTexMat", filename.c_str());

	// Load Spline Material
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/spline");
	resman_->LoadResource(Material, "SplineMaterial", filename.c_str());

	// Load material to be applied to particles
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/particle");
	resman_->LoadResource(Material, "ParticleMaterial", filename.c_str());

	filename = std::string(MATERIAL_DIRECTORY) + std::string("/flame_particle");
	resman_->LoadResource(Material, "FlameMaterial", filename.c_str());



	// Load a player model from a file
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/body.obj");
	resman_->LoadResource(Mesh, "PlayerMesh", filename.c_str());

	// Load ship blade for player model
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/shipBlade.obj");
	resman_->LoadResource(Mesh, "shipBlade", filename.c_str());

	// Load a laser from a file
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/missilev2.obj");
	resman_->LoadResource(Mesh, "MissileMesh", filename.c_str());

	filename = std::string(MATERIAL_DIRECTORY) + std::string("/tankv2.obj");
	resman_->LoadResource(Mesh, "TankMesh", filename.c_str());

	filename = std::string(MATERIAL_DIRECTORY) + std::string("/towerBase.obj");
	resman_->LoadResource(Mesh, "TowerMesh", filename.c_str());

	// Load a cube from a file
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/cube.obj");
	resman_->LoadResource(Mesh, "OtherMesh", filename.c_str());



	// Load window texture
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/window.jpg");
	resman_->LoadResource(Texture, "Window", filename.c_str());

	filename = std::string(MATERIAL_DIRECTORY) + std::string("/tower.png");
	resman_->LoadResource(Texture, "TowerTexture", filename.c_str());

	filename = std::string(MATERIAL_DIRECTORY) + std::string("/tank.jpg");
	resman_->LoadResource(Texture, "TankTexture", filename.c_str());

	// Load metal texture
	filename = std::string(MATERIAL_DIRECTORY) + std::string("/metal.jpg");
	resman_->LoadResource(Texture, "Metal", filename.c_str());

	filename = std::string(MATERIAL_DIRECTORY) + std::string("/orbact.jpg");
	resman_->LoadResource(Texture, "OrbTextureAct", filename.c_str());

	filename = std::string(MATERIAL_DIRECTORY) + std::string("/orbalt.jpg");
	resman_->LoadResource(Texture, "OrbTextureAlt", filename.c_str());

	filename = std::string(MATERIAL_DIRECTORY) + std::string("/laser.png");
	resman_->LoadResource(Texture, "Laser", filename.c_str());

	filename = std::string(MATERIAL_DIRECTORY) + std::string("/floor.png");
	resman_->LoadResource(Texture, "Floor", filename.c_str());

	filename = std::string(MATERIAL_DIRECTORY) + std::string("/welcome.png");
	resman_->LoadResource(Texture, "WelcomeTexture", filename.c_str());


	resman_->CreateSphereParticles("SphereParticles");
	resman_->CreateFlameParticles("FlameParticles");
}


void Game::SetupScene(void){

	// Set background color for the scene
	scene_->SetBackgroundColor(viewport_background_color_g);
	enemies_pos_ = glm::vec3(0, -250, -1250);


	CreatePlayerInstance("PlayerInstance", "PlayerMesh", SHINY_BLUE_MATERIAL);

	CreateLand(glm::vec3(1, 1, 1), glm::vec3(0.0, 0.0, -25.0), glm::vec3(10.0, 10.0, 10.0), "WelcomeTexture");
	CreateLand(glm::vec3(10, 1, 10), enemies_pos_ + glm::vec3(-500.0, -50.0, -500.0), glm::vec3(100.0, 100.0, 100.0), "Floor");

	//scene_->AddParticle(particle);

	tower_control_->init(enemies_pos_);
	tank_control_->init(enemies_pos_);
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
			node = scene_->GetNode("PlayerInstance");

			update(node, delta_time);
        }

        // Draw the scene
        scene_->Draw(&camera_, scene_->GetPlayer()->getPos());
		tower_control_->draw(&camera_, scene_->GetPlayer()->getPos());
		tank_control_->draw(&camera_, scene_->GetPlayer()->getPos());

        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);

        // Update other events like input handling
        glfwPollEvents();
    }
}

void Game::update(SceneNode* node, double delta_time) {
	input(node, delta_time);

	scene_->Update(delta_time);
	glm::vec3 pos = scene_->GetPlayer()->getPos();
	if (pos.x > enemies_pos_.x - 550 && pos.x < enemies_pos_.x + 450 && pos.z < enemies_pos_.z + 450 && pos.z > enemies_pos_.z - 550) {
		tower_control_->update(delta_time, scene_->GetPlayer());
		tank_control_->update(delta_time, scene_->GetPlayer());
	}
}

void Game::input(SceneNode* node, double delta_time) {
	float roll_factor = glm::radians(2000.0) * delta_time;
	float trans_factor = 25.0 * delta_time;
	float camera_factor = 10.0;

	//Move Forward
	if (key_[GLFW_KEY_W] == GLFW_PRESS || key_[GLFW_KEY_W] == GLFW_REPEAT) {
		node->Translate(camera_.GetForward() * trans_factor);
		camera_.Translate(camera_.GetForward() * trans_factor);
	}
	//Move Backward
	if (key_[GLFW_KEY_S] == GLFW_PRESS || key_[GLFW_KEY_S] == GLFW_REPEAT) {
		node->Translate(camera_.GetForward() * -trans_factor);
		camera_.Translate(camera_.GetForward() * -trans_factor);
	}
	//Move Left
	if (key_[GLFW_KEY_A] == GLFW_PRESS || key_[GLFW_KEY_A] == GLFW_REPEAT) {
		node->Translate(camera_.GetSide() * -trans_factor);
		camera_.Translate(camera_.GetSide() * -trans_factor);
	}
	//Move Right
	if (key_[GLFW_KEY_D] == GLFW_PRESS || key_[GLFW_KEY_D] == GLFW_REPEAT) {
		node->Translate(camera_.GetSide() * trans_factor);
		camera_.Translate(camera_.GetSide() * trans_factor);
	}
	//Move Upwards
	if (key_[GLFW_KEY_Q] == GLFW_PRESS || key_[GLFW_KEY_Q] == GLFW_REPEAT) {
		node->Translate(camera_.GetUp() * trans_factor);
		camera_.Translate(camera_.GetUp() * trans_factor);
	}
	//Move Downwards
	if (key_[GLFW_KEY_E] == GLFW_PRESS || key_[GLFW_KEY_E] == GLFW_REPEAT) {
		node->Translate(camera_.GetUp() * -trans_factor);
		camera_.Translate(camera_.GetUp() * -trans_factor);
	}
	//Roll Left
	if (key_[GLFW_KEY_Z] == GLFW_PRESS || key_[GLFW_KEY_Z] == GLFW_REPEAT) {
		node->Rotate(glm::angleAxis((float)glm::radians(1.0), glm::vec3(0.0, 0.0, roll_factor)));
		if (scene_->GetPlayer()->get_first_person() == false) {
			camera_.Translate(camera_.GetForward() * camera_factor);
		}
		camera_.Rotate(glm::angleAxis((float)glm::radians(1.0), glm::vec3(0.0, 0.0, roll_factor)));
		if (scene_->GetPlayer()->get_first_person() == false) {
			camera_.Translate(camera_.GetForward() * -camera_factor);
		}
	}
	//Roll Right
	if (key_[GLFW_KEY_C] == GLFW_PRESS || key_[GLFW_KEY_C] == GLFW_REPEAT) {
		node->Rotate(glm::angleAxis((float)glm::radians(1.0), glm::vec3(0.0, 0.0, -roll_factor)));
		if (scene_->GetPlayer()->get_first_person() == false) {
			camera_.Translate(camera_.GetForward() * camera_factor);
		}
		camera_.Rotate(glm::angleAxis((float)glm::radians(1.0), glm::vec3(0.0, 0.0, -roll_factor)));
		if (scene_->GetPlayer()->get_first_person() == false) {
			camera_.Translate(camera_.GetForward() * -camera_factor);
		}
	}
	//Roll Forward
	if (key_[GLFW_KEY_DOWN] == GLFW_PRESS || key_[GLFW_KEY_DOWN] == GLFW_REPEAT) {
		node->Rotate(glm::angleAxis((float)glm::radians(1.0), glm::vec3(-roll_factor, 0.0, 0.0)));
		if (scene_->GetPlayer()->get_first_person() == false) {
			camera_.Translate(camera_.GetForward() * camera_factor);
		}
		camera_.Rotate(glm::angleAxis((float)glm::radians(1.0), glm::vec3(-roll_factor, 0.0, 0.0)));
		if (scene_->GetPlayer()->get_first_person() == false) {
			camera_.Translate(camera_.GetForward() * -camera_factor);
		}
	}
	//Roll Back
	if (key_[GLFW_KEY_UP] == GLFW_PRESS || key_[GLFW_KEY_UP] == GLFW_REPEAT) {
		node->Rotate(glm::angleAxis((float)glm::radians(1.0), glm::vec3(roll_factor, 0.0, 0.0)));
		if (scene_->GetPlayer()->get_first_person() == false) {
			camera_.Translate(camera_.GetForward() * camera_factor);
		}
		camera_.Rotate(glm::angleAxis((float)glm::radians(1.0), glm::vec3(roll_factor, 0.0, 0.0)));
		if (scene_->GetPlayer()->get_first_person() == false) {
			camera_.Translate(camera_.GetForward() * -camera_factor);
		}
	}
	//Turn Left
	if (key_[GLFW_KEY_LEFT] == GLFW_PRESS || key_[GLFW_KEY_LEFT] == GLFW_REPEAT) {
		node->Rotate(glm::angleAxis((float)glm::radians(1.0), glm::vec3(0.0, roll_factor, 0.0)));
		if (scene_->GetPlayer()->get_first_person() == false) {
			camera_.Translate(camera_.GetForward() * camera_factor);
		}
		camera_.Rotate(glm::angleAxis((float)glm::radians(1.0), glm::vec3(0.0, roll_factor, 0.0)));
		if (scene_->GetPlayer()->get_first_person() == false) {
			camera_.Translate(camera_.GetForward() * -camera_factor);
		}
	}
	//Turn Right
	if (key_[GLFW_KEY_RIGHT] == GLFW_PRESS || key_[GLFW_KEY_RIGHT] == GLFW_REPEAT) {
		node->Rotate(glm::angleAxis((float)glm::radians(1.0), glm::vec3(0.0, -roll_factor, 0.0)));
		if (scene_->GetPlayer()->get_first_person() == false) {
			camera_.Translate(camera_.GetForward() * camera_factor);
		}
		camera_.Rotate(glm::angleAxis((float)glm::radians(1.0), glm::vec3(0.0, -roll_factor, 0.0)));
		if (scene_->GetPlayer()->get_first_person() == false) {
			camera_.Translate(camera_.GetForward() * -camera_factor);
		}
	}
}


void Game::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){

	// Get user data with a pointer to the game class
	void* ptr = glfwGetWindowUserPointer(window);
	Game *game = (Game *)ptr;

	game->key_[key] = action;

	float camera_factor = 10.0;

	// Quit game if 'q' is pressed
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	
	if (key == GLFW_KEY_V && action == GLFW_PRESS) {
		game->scene_->GetPlayer()->toggle_first_person();
		if (game->scene_->GetPlayer()->get_first_person() == true) {
			game->camera_.Translate(game->camera_.GetForward() * camera_factor);
		}
		else {
			game->camera_.Translate(game->camera_.GetForward() * -camera_factor);
		}
	}

	//Fire Laser
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		//Laser *lsr = createLaserInstance(game->resman_);
		//lsr->SetInitPos(game->scene_->GetPlayer()->getPos());
		//lsr->SetOrientation(game->camera_.GetOrientation());
		//lsr->SetSpeed(10.0);
		//game->lasers_.push_back(lsr);
		game->scene_->GetPlayer()->addLaser(&game->camera_);
	}

	//Fire Bomb
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		//Bomb *bmb = createBombInstance(game->resman_);
		//bmb->Translate(game->scene_->GetPlayer()->getPos());
		//bmb->SetSpeed(-1.0);
		//bmb->SetTimer(5.0);
		//game->bombs_.push_back(bmb);
		game->scene_->GetPlayer()->addBomb();
	}

	//Fire Missile
	if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
		//Missile *msl = createMissileInstance(game->resman_);
		//msl->SetInitPos(game->scene_->GetPlayer()->getPos());
		//msl->SetOrientation(game->camera_.GetOrientation());
		//msl->SetSpeed(10.0);
		//msl->setPoints(game->camera_.GetForward(), game->camera_.GetUp());
		//game->missiles_.push_back(msl);
		game->scene_->GetPlayer()->addMissile(&game->camera_);
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

	//CreatePlayerInstance("PlayerInstance", "PlayerMesh", SHINY_BLUE_MATERIAL);
    // Get resources
    Resource *geom = getResource(resman_, "PlayerMesh");
    Resource *mat = getResource(resman_, SHINY_TEXTURE_MATERIAL);
	Resource *tex = resman_->GetResource("Window");

    // Create Player instance
    Player *player = new Player("PlayerInstance", geom, mat, tex);
	player->setResman(resman_);

    SceneNode *rotor = CreateInstance("PlayerRotor", "shipBlade", SHINY_TEXTURE_MATERIAL, "Window");
    rotor->Scale(glm::vec3(0.20, 0.20, 0.20));
	//upper_body->Rotate(glm::angleAxis((float) glm::radians(1.0), glm::vec3(15.0, 0.0, 0.0)));
    rotor->Translate(glm::vec3(0.0, -0.35, 0));

	SceneNode *particle = createParticleInstance(resman_, "FlameParticles", "FlameMaterial");
	particle->SetOrientation(glm::angleAxis((float)glm::radians(90.0f), glm::vec3(1, 0, 0)));
	particle->SetScale(glm::vec3(1.0, 1.0, 4.0));
	particle->SetReset(1.0);

	player->addChild(rotor);
	player->addChild(particle);
    scene_->AddNode(player);
	scene_->AddPlayer(player);
    return player;
}


void Game::CreateLand(glm::vec3 size, glm::vec3 pos, glm::vec3 scale, std::string texture_name){
	game::SceneNode *node;
    // Create a number of asteroid instances
	for (int x = 0; x < size.x; ++x) {
		for (int y = 0; y < size.y; ++y) {
			for (int z = 0; z < size.z; ++z) {
				node = CreateInstance("Land", "CubeMesh", "BrightTexMat", texture_name);
				node->Scale(scale);
				node->Translate(pos + glm::vec3(scale.x*x, scale.y*y, scale.z*z));
			}
		}
	}
}


SceneNode *Game::CreateInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name){

    Resource *geom = resman_->GetResource(object_name);
    if (!geom){
        throw(GameException(std::string("Could not find resource \"")+object_name+std::string("\"")));
    }

    Resource *mat = resman_->GetResource(material_name);
    if (!mat){
        throw(GameException(std::string("Could not find resource \"")+material_name+std::string("\"")));
    }

    Resource *tex = resman_->GetResource(texture_name);

    SceneNode *scn = scene_->CreateNode(entity_name, geom, mat, tex);
    return scn;
}

SceneNode *Game::CreateParticleInstanceV(std::string object_name, std::string material_name, std::string texture_name){
	SceneNode* particle = createParticleInstance(resman_, object_name, material_name, texture_name);
	scene_->AddParticle(particle);

    return particle;
}

Laser *Game::CreateLaserInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name) {

	Resource *geom = resman_->GetResource(object_name);
	if (!geom) {
		throw(GameException(std::string("Could not find resource \"") + object_name + std::string("\"")));
	}

	Resource *mat = resman_->GetResource(material_name);
	if (!mat) {
		throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
	}

	Resource *tex = resman_->GetResource(texture_name);

	Laser *lsr = new Laser(entity_name, geom, mat, tex);
	lsr->Scale(glm::vec3(0.15, 0.15, 1.0));
	scene_->AddNode(lsr);
	return lsr;
}

Bomb *Game::CreateBombInstance(std::string entity_name, std::string object_name, std::string material_name, std::string texture_name) {

	Resource *geom = resman_->GetResource(object_name);
	if (!geom) {
		throw(GameException(std::string("Could not find resource \"") + object_name + std::string("\"")));
	}

	Resource *mat = resman_->GetResource(material_name);
	if (!mat) {
		throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
	}

	Resource *tex = resman_->GetResource(texture_name);

	Bomb *bmb = new Bomb(entity_name, geom, mat, tex);
	bmb->Scale(glm::vec3(0.15, 0.15, 0.15));
	scene_->AddNode(bmb);
	return bmb;
}

} // namespace game
