#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "tank_control.h"
#include "misc.h"

namespace game {

TankControl::TankControl(ResourceManager *rm) : resman_(rm) { }

TankControl::~TankControl(){
}

void TankControl::update(double delta_time, glm::vec3 player_pos){
	//player_pos = glm::vec3(10.0, 0.0, 11.0);

	for each (Tank *tank in tanks_) {
		tank->Update(delta_time);

		if (tank->move()) {
			if (glm::length(glm::vec3(tank->GetPosition().x, 0, tank->GetPosition().z) - glm::vec3(player_pos.x, 0, player_pos.z)) > tank->getMoveError()*2)
				move(tank, player_pos);
			else {
				if (tank->shoot())
					shoot(tank, player_pos);
			}
		}
	}

	for each (Laser *lsr in lasers_) {
		lsr->Update(delta_time);
	}
}

void TankControl::move(Tank *tank, glm::vec3 player_pos) {
	if (glm::length(glm::vec3(tank->GetPosition().x, 0, tank->GetPosition().z) - glm::vec3(player_pos.x, 0, player_pos.z)) > pow(tank->getMoveError(), 2)) {
		std::cout << "tank moveing!!!" << std::endl;
		//std::cout << "pos: " << firing_error.x << ", " << firing_error.y << ", " << firing_error.z << std::endl;

		tank->setDestination(glm::vec2(player_pos.x + tank->moveError(), player_pos.z + tank->moveError()));
	}
}

void TankControl::shoot(Tank *tank, glm::vec3 player_pos) {
	//std::cout << "fired a shot!!!" << std::endl;
	glm::vec3 firing_error = glm::vec3(tank->fireError(), tank->fireError(), tank->fireError());
	//std::cout << "pos: " << firing_error.x << ", " << firing_error.y << ", " << firing_error.z << std::endl;

	player_pos += firing_error;

	//player_pos
	Laser *lsr = createLaserInstance(resman_);
	lsr->SetOrientation(glm::conjugate(glm::toQuat(glm::lookAt(tank->getPos(), player_pos, glm::vec3(0.0, 1.0, 0.0)))));
	lsr->SetInitPos(tank->getPos());
	lsr->SetSpeed(tank->getLaserSpeed());
	lasers_.push_back(lsr);
}

void TankControl::init() {
	for (int i = 0; i < 25; ++i) {
		createTankInstance(glm::vec3(rand() % 1000 - 500, 0, rand() % 1000 - 500));
	}
}

void TankControl::draw(Camera *camera) {
	for each (Tank *tank in tanks_) {
		tank->Draw(camera);
	}

	for each (Laser *lsr in lasers_) {
		lsr->Draw(camera);
	}
}

Tank *TankControl::createTankInstance(glm::vec3 pos) {
	Resource *geom = getResource(resman_, "CubeMesh");
	Resource *mat = getResource(resman_, "ShinyTextureMaterial");
	Resource *tex = resman_->GetResource("");

	Tank *tank = new Tank("Tank", geom, mat, tex);
	tank->Scale(glm::vec3(1.0, 1.0, 1.0));
	tank->Translate(pos + glm::vec3(0.0, 0.5, 0.0));
	tank->setLaserSpeed(rand() % 5 + 5);
	tank->setFireError(25);
	tank->setFireSpeed(10);

	tanks_.push_back(tank);
	return tank;
}
            
} // namespace game
