#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "tank_control.h"
#include "misc.h"
#include "player.h"

namespace game {

TankControl::TankControl(ResourceManager *rm) : resman_(rm) { }

TankControl::~TankControl(){
}

void TankControl::update(double delta_time, Player* player){
	glm::vec3 player_pos = player->getPos();

	for (int i = 0; i < bomb_particles_.size(); ++i) {
		bomb_particles_[i]->Update(delta_time);
		if (bomb_particles_[i]->done())
			bomb_particles_.erase(bomb_particles_.begin() + i);
		else {
			if (collision(bomb_particles_[i], player)) {
				player->takeDamage(bomb_particles_[i]->GetExpDamage() * delta_time);
				//std::cout << "its working!!!" << std::endl;
			}
		}
	}

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

	//for each (Bomb *bmb in bombs_) {
	for (int i = 0; i < bombs_.size(); ++i) {
		bombs_[i]->Update(delta_time);
		//std::cout << "pos: " << bombs_[i]->GetPosition().x << ", " << bombs_[i]->GetPosition().y << ", " << bombs_[i]->GetPosition().z << std::endl;

		if (bombs_[i]->Explode() || collision(bombs_[i], player)) {
			SceneNode* particle = createParticleInstance(resman_, "SphereParticles", "ParticleMaterial");
			particle->SetPosition(bombs_[i]->GetPosition());
			float rad = bombs_[i]->GetExpRadius()/SPEHRE_PARTICLE_SPEED;
			std::cout << "\t\trad: " << rad << std::endl;
			particle->SetReset(bombs_[i]->GetExpRadius()/SPEHRE_PARTICLE_SPEED);
			particle->SetExpDamage(bombs_[i]->GetDamage());

			bomb_particles_.push_back(particle);
			bombs_.erase(bombs_.begin() + i);
			//std::cout << "exploding is true!!!" << std::endl;
		}
	}
}

void TankControl::move(Tank *tank, glm::vec3 player_pos) {
	if (glm::length(glm::vec3(tank->GetPosition().x, 0, tank->GetPosition().z) - glm::vec3(player_pos.x, 0, player_pos.z)) > pow(tank->getMoveError(), 2)) {
		//std::cout << "tank moveing!!!" << std::endl;
		//std::cout << "pos: " << firing_error.x << ", " << firing_error.y << ", " << firing_error.z << std::endl;

		tank->setDestination(glm::vec2(player_pos.x + tank->moveError(), player_pos.z + tank->moveError()));
	}
}

void TankControl::shoot(Tank *tank, glm::vec3 player_pos) {
	//std::cout << "fired a shot!!!" << std::endl;
	//glm::vec3 firing_error = glm::vec3(tank->fireError(), tank->fireError(), tank->fireError());
	//std::cout << "pos: " << firing_error.x << ", " << firing_error.y << ", " << firing_error.z << std::endl;

	//player_pos += firing_error;

	//player_pos
	Bomb *bmb = createBombInstance(resman_);
	//bmb->SetOrientation(glm::conjugate(glm::toQuat(glm::lookAt(tank->getPos(), player_pos, glm::vec3(0.0, 1.0, 0.0)))));
	bmb->SetPosition(tank->getPos() + glm::vec3(0, 3.0, 0));
	bmb->SetTimer(tank->getBombTimer());
	bmb->SetSpeed(tank->getBombSpeed());
	bmb->SetDamage(tank->GetBombDamage());
	bmb->SetExpRadius(1.0);
	//std::cout << "bombing is shooting::: " << tank->getBombSpeed() << std::endl;
	bombs_.push_back(bmb);
}

void TankControl::init() {
	//for (int i = 0; i < 25; ++i) {
	for (int i = 0; i < 1; ++i) {
		createTankInstance(glm::vec3(rand() % 1000 - 500, 0, rand() % 1000 - 500));
	}
}

void TankControl::draw(Camera *camera) {
	for each (SceneNode *particle in bomb_particles_) {
		particle->Draw(camera);
	}

	for each (Tank *tank in tanks_) {
		tank->Draw(camera);
	}

	for each (Bomb *bmb in bombs_) {
		bmb->Draw(camera);
	}
}

Tank *TankControl::createTankInstance(glm::vec3 pos) {
	Resource *geom = getResource(resman_, "TankMesh");
	Resource *mat = getResource(resman_, "ShinyBlueMaterial");
	Resource *tex = resman_->GetResource("");

	Tank *tank = new Tank("Tank", geom, mat, tex);
	tank->Scale(glm::vec3(1.0, 1.0, 1.0));
	tank->Translate(pos + glm::vec3(0.0, 1.68, 0.0));
	//tank->setBombSpeed(rand() % 5 + 5);
	//tank->setFireError(25);
	tank->setFireSpeed(10);
	tank->SetBombDamage(rand() % 5 + 5);
	tank->SetExpDamage(5);

	tanks_.push_back(tank);
	return tank;
}
            
} // namespace game
