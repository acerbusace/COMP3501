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

	//for each (SceneNode* mine in mines_) {
	for(int i = 0; i < mines_.size(); ++i) {
		mines_timer_[i] -= delta_time;

		if (glm::length(player_pos - mines_[i]->getPos()) < 20 && mines_timer_[i] < 0) {
			SceneNode* particle = createParticleInstance(resman_, "SphereParticles", "ParticleMaterial");
			particle->SetPosition(mines_[i]->getPos());
			particle->SetReset(25.0);
			particle->SetExpDamage(25);

			mines_timer_[i] = mines_reset_[i];
			bomb_particles_.push_back(particle);
		}
	}

	//for each (Tank *tank in tanks_) {
	for (int i = 0; i < tanks_.size(); ++i) {
		tanks_[i]->Update(delta_time);

		if (tanks_[i]->move()) {
			if (glm::length(glm::vec3(tanks_[i]->GetPosition().x, 0, tanks_[i]->GetPosition().z) - glm::vec3(player_pos.x, 0, player_pos.z)) > tanks_[i]->getMoveError()*2)
				move(tanks_[i], player_pos);
			else {
				if (tanks_[i]->shoot())
					shoot(tanks_[i], player_pos);
			}
		}

		std::vector<Laser*> *lasers = player->getLasers();
		for (int j = 0; j < lasers->size(); ++j) {
			if (collision(lasers->at(j), tanks_[i])) {
				if (tanks_[i]->takeDamage(25)) {
					SceneNode* particle = createParticleInstance(resman_, "SphereParticles", "ParticleMaterial");
					particle->SetPosition(tanks_[i]->GetPosition());
					particle->SetReset(5.0);
					particle->SetExpDamage(tanks_[i]->GetBodyDamage());

					bomb_particles_.push_back(particle);

					tanks_.erase(tanks_.begin() + i);
					break;
				}
				lasers->erase(lasers->begin() + j);
			}
		}

		std::vector<Missile*> *missiles = player->getMissiles();
		for (int j = 0; j < missiles->size(); ++j) {
			if (collision(missiles->at(j), tanks_[i])) {
				if (tanks_[i]->takeDamage(50)) {
					SceneNode* particle = createParticleInstance(resman_, "SphereParticles", "ParticleMaterial");
					particle->SetPosition(tanks_[i]->GetPosition());
					particle->SetReset(5.0);
					particle->SetExpDamage(tanks_[i]->GetBodyDamage());

					bomb_particles_.push_back(particle);

					tanks_.erase(tanks_.begin() + i);
					break;
				}
				missiles->erase(missiles->begin() + j);
			}
		}

		std::vector<Bomb*> *bombs = player->getBombs();
		for (int j = 0; j < bombs->size(); ++j) {
			if (collision(bombs->at(j), tanks_[i])) {
				player->addBombParticle(bombs->at(j)->GetPosition());

				bombs->erase(bombs->begin() + j);
			}
		}

		std::vector<SceneNode*> *bomb_particles = player->getBombParticles();
		for (int j = 0; j < bomb_particles->size(); ++j) {
			if (collision(bomb_particles->at(j), tanks_[i])) {
				if (tanks_[i]->takeDamage(bomb_particles->at(j)->GetExpDamage() * delta_time)) {
					SceneNode* particle = createParticleInstance(resman_, "SphereParticles", "ParticleMaterial");
					particle->SetPosition(tanks_[i]->GetPosition());
					particle->SetReset(5.0);
					particle->SetExpDamage(tanks_[i]->GetBodyDamage());

					bomb_particles_.push_back(particle);

					tanks_.erase(tanks_.begin() + i);
					break;
				}
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
	bmb->SetPosition(tank->getPos() + glm::vec3(0, 3.25, 0));
	bmb->SetTimer(tank->getBombTimer());
	bmb->SetSpeed(tank->getBombSpeed());
	bmb->SetDamage(tank->GetBombDamage());
	bmb->SetExpRadius(1.0);
	//std::cout << "bombing is shooting::: " << tank->getBombSpeed() << std::endl;
	bombs_.push_back(bmb);
}

void TankControl::init(glm::vec3 pos) {
	//for (int i = 0; i < 25; ++i) {
	for (int i = 0; i < 25; ++i) {
		createTankInstance(pos + glm::vec3(rand() % 900 - 450, 0, rand() % 900 - 450));
	}

	for (int i = 0; i < 25; ++i) {
		createMineInstance(pos + glm::vec3(rand() % 900 - 450, rand() % 100, rand() % 900 - 450));
	}
}

void TankControl::draw(Camera *camera, glm::vec3 light_pos) {
	for each (SceneNode *particle in bomb_particles_) {
		particle->Draw(camera, light_pos);
	}

	for each (Tank *tank in tanks_) {
		tank->Draw(camera, light_pos);
	}

	for each (Bomb *bmb in bombs_) {
		bmb->Draw(camera, light_pos);
	}

	for each (SceneNode *mine in mines_) {
		mine->Draw(camera, light_pos);
	}
}

Tank *TankControl::createTankInstance(glm::vec3 pos) {
	Resource *geom = getResource(resman_, "TankMesh");
	Resource *mat = getResource(resman_, "ShinyTextureMaterial");
	Resource *tex = resman_->GetResource("TankTexture");

	Tank *tank = new Tank("Tank", geom, mat, tex);
	tank->Scale(glm::vec3(1.0, 1.0, 1.0));
	tank->Translate(pos + glm::vec3(0.0, 2.15, 0.0));
	//tank->setBombSpeed(rand() % 5 + 5);
	//tank->setFireError(25);
	tank->setFireSpeed(10);
	tank->SetBombDamage(rand() % 5 + 5);
	tank->SetExpDamage(5);

	tanks_.push_back(tank);
	return tank;
}

SceneNode *TankControl::createMineInstance(glm::vec3 pos) {
	Resource *geom = getResource(resman_, "PlayerMesh");
	Resource *mat = getResource(resman_, "ShinyTextureMaterial");
	Resource *tex = resman_->GetResource("TankTexture");

	SceneNode *mine = new SceneNode("Mine", geom, mat, tex);
	mine->Scale(glm::vec3(1.0, 1.0, 1.0));
	mine->Translate(pos);

	mines_.push_back(mine);
	mines_reset_.push_back(15);
	mines_timer_.push_back(0);
	return mine;
}
            
} // namespace game
