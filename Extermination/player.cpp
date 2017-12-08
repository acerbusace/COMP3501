#include "player.h"
#include "laser.h"
#include "bomb.h"
#include "missile.h"
#include "camera.h"
#include "resource_manager.h"
#include "misc.h"
#include <iostream>

namespace game {

Player::Player(std::string name, Resource *geometry, Resource *material, Resource *texture) : SceneNode(name, geometry, material, texture) {
	first_person_ = false;
	health_ = 1000;
}


Player::~Player() {
}


void Player::Draw(Camera *camera, glm::vec3 light_pos) {
	if (first_person_ == false) {
		SceneNode::Draw(camera, light_pos);
	}

	for each (Bomb *bmb in bombs_) {
		bmb->Draw(camera, light_pos);
	}
	for each (Laser *lsr in lasers_) {
		lsr->Draw(camera, light_pos);
	}
	for each (Missile *msl in missiles_) {
		msl->Draw(camera, light_pos);
	}
	for each (SceneNode *particle in bomb_particles_) {
		particle->Draw(camera, light_pos);
	}
}


void Player::Update(double delta_time) {
	SceneNode *rotor = children[0];

	rotor->Rotate(glm::angleAxis((float)(glm::radians(90.0) * delta_time), glm::vec3(0.0, 1.0, 0.0)));

	for (int i = 0; i < bombs_.size(); ++i) {
		bombs_[i]->Update(delta_time);

		if (bombs_[i]->Explode()) {
			addBombParticle(bombs_[i]->GetPosition());
			bombs_.erase(bombs_.begin() + i);
		}
	}

	for (int i = 0; i < lasers_.size(); ++i) {
		lasers_[i]->Update(delta_time);
		if (lasers_[i]->done()) {
			lasers_.erase(lasers_.begin() + i);
		}
	}

	for (int i = 0; i < missiles_.size(); ++i) {
		missiles_[i]->Update(delta_time);
		if (missiles_[i]->done()) {
			missiles_.erase(missiles_.begin() + i);
		}
	}

	for (int i = 0; i < bomb_particles_.size(); ++i) {
		bomb_particles_[i]->Update(delta_time);
		if (bomb_particles_[i]->done()) {
			bomb_particles_.erase(bomb_particles_.begin() + i);
		}
	}
}


void Player::addLaser(Camera *camera) {
	Laser *lsr = createLaserInstance(resman_);
	lsr->SetInitPos(getPos());
	lsr->SetOrientation(camera->GetOrientation());
	lsr->SetSpeed(50.0);
	lasers_.push_back(lsr);
}


void Player::addBomb() {
	Bomb *bmb = createBombInstance(resman_);
	bmb->Translate(getPos());
	bmb->SetSpeed(-1.75);
	bmb->SetTimer(5.0);
	bombs_.push_back(bmb);
}


void Player::addMissile(Camera *camera) {
	Missile *msl = createMissileInstance(resman_);
	msl->SetInitPos(getPos());
	msl->SetOrientation(camera->GetOrientation());
	msl->SetSpeed(10.0);
	msl->setPoints(camera->GetForward(), camera->GetUp());

	SceneNode *particle = createParticleInstance(resman_, "FlameParticles", "FlameMaterial");
	particle->SetOrientation(glm::angleAxis((float)glm::radians(90.0f), glm::vec3(1, 0, 0)));
	particle->SetScale(glm::vec3(2.0, 2.0, 4.0));
	particle->SetReset(1.0);

	msl->addChild(particle);
	missiles_.push_back(msl);
}


void Player::addBombParticle(glm::vec3 pos) {
	SceneNode* particle = createParticleInstance(resman_, "SphereParticles", "ParticleMaterial");
	particle->SetReset(5.0);
	particle->SetPosition(pos);
	particle->SetExpDamage(25.0);

	bomb_particles_.push_back(particle);
}


std::vector<Laser*>* Player::getLasers() {
	return &lasers_;
}


std::vector<Bomb*>* Player::getBombs() {
	return &bombs_;
}


std::vector<Missile*>* Player::getMissiles() {
	return &missiles_;
}


std::vector<SceneNode*>* Player::getBombParticles() {
	return &bomb_particles_;
}


bool Player::get_first_person() {
	return first_person_;
}


void Player::toggle_first_person() {
	first_person_ = (first_person_ == true) ? false : true;
}


bool Player::takeDamage(float damage) {
	health_ -= damage;
	std::cout << "Player Health: " << health_ << std::endl;

	if (health_ <= 0)
		return true;
	return false;
}


void Player::setResman(ResourceManager *resman) {
	resman_ = resman;
}

} // namespace game
