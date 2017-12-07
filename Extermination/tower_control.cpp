#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "tower_control.h"
#include "misc.h"

namespace game {

TowerControl::TowerControl(ResourceManager *rm) : resman_(rm) { }

TowerControl::~TowerControl(){
}

void TowerControl::update(double delta_time, Player* player){
	glm::vec3 player_pos = player->getPos();

	//for each (Orb *orb in orbs_) {
	for (int i = 0; i < orbs_.size(); ++i) {
		bool fallen = false;
		if (!orbs_[i]->done()) {
			orbs_[i]->Update(delta_time);

			if (orbs_[i]->shoot())
				shoot(orbs_[i], player_pos);

			std::vector<Laser*> *lasers = player->getLasers();
			for (int j = 0; j < lasers->size(); ++j) {
				if (collision(lasers->at(j), orbs_[i])) {
					std::cout << "orb collision" << std::endl;
					if (orbs_[i]->takeDamage(25)) {
						fallen = true;
						break;
					}
					lasers->erase(lasers->begin() + j);
				}
			}
			if (fallen) continue;

			std::vector<Missile*> *missiles = player->getMissiles();
			for (int j = 0; j < missiles->size(); ++j) {
				if (collision(missiles->at(j), orbs_[i])) {
					std::cout << "orb collision" << std::endl;
					if (orbs_[i]->takeDamage(50)) {
						fallen = true;
						break;
					}
					missiles->erase(missiles->begin() + j);
				}
			}
			if (fallen) continue;

			std::vector<Bomb*> *bombs = player->getBombs();
			for (int j = 0; j < bombs->size(); ++j) {
				if (collision(bombs->at(j), orbs_[i])) {
					std::cout << "orb collision" << std::endl;
					player->addBombParticle(bombs->at(j)->GetPosition());

					bombs->erase(bombs->begin() + j);
				}
			}

			std::vector<SceneNode*> *bomb_particles = player->getBombParticles();
			for (int j = 0; j < bomb_particles->size(); ++j) {
				if (collision(bomb_particles->at(j), orbs_[i])) {
					if (orbs_[i]->takeDamage(bomb_particles->at(j)->GetExpDamage() * delta_time)) {
						fallen = true;
						break;
					}
				}
			}
			if (fallen) continue;

		}
	}

	//for each (Laser *lsr in lasers_) {
	for (int i = 0; i < lasers_.size(); ++i) {
		lasers_[i]->Update(delta_time);

		//if (collision(lasers_[i], player)) {
		//	player->takeDamage(lasers_[i]->GetDamage());
		//	lasers_.erase(lasers_.begin() + i);
		//} else if (lasers_[i]->done()) {
		//	lasers_.erase(lasers_.begin() + i);
		//}
	}
}

void TowerControl::shoot(Orb *orb, glm::vec3 player_pos) {
	//std::cout << "fired a shot!!!" << std::endl;
	glm::vec3 firing_error = glm::vec3(orb->fireError(), orb->fireError(), orb->fireError());
	//std::cout << "pos: " << firing_error.x << ", " << firing_error.y << ", " << firing_error.z << std::endl;

	player_pos += firing_error;

	 //player_pos
	Laser *lsr = createLaserInstance(resman_);
	lsr->SetOrientation(glm::conjugate(glm::toQuat(glm::lookAt(orb->getPos(), player_pos, glm::vec3(0.0, 1.0, 0.0)))));
	lsr->SetInitPos(orb->getPos());
	lsr->SetSpeed(orb->getLaserSpeed());
	lsr->SetDamage(orb->GetLaserDamage());
	lasers_.push_back(lsr);
}

void TowerControl::init(glm::vec3 pos) {
	for (int i = 0; i < 15; ++i) {
		createTowerInstance(pos + glm::vec3(rand() % 900 - 450, 0, rand() % 900 - 450));
	}
}

void TowerControl::draw(Camera *camera, glm::vec3 light_pos) {
	for each (Tower *twr in towers_) {
		twr->Draw(camera, light_pos);
	}

	for each (Laser *lsr in lasers_) {
		lsr->Draw(camera, light_pos);
	}
}

Tower *TowerControl::createTowerInstance(glm::vec3 pos) {
	Resource *geom = getResource(resman_, "TowerMesh");
	Resource *mat = getResource(resman_, "ShinyTextureMaterial");
	Resource *tex = resman_->GetResource("TowerTexture");
	//Resource *tex = resman_->GetResource("Window");

	Tower *twr = new Tower("Tower", geom, mat, tex);
	twr->Scale(glm::vec3(1.5, 3.0, 1.5));
	twr->Translate(pos + glm::vec3(0.0, 0.0, 0.0));

	geom = getResource(resman_, "SphereMesh");
	mat = getResource(resman_, "ShinyTextureMaterial");
	tex = resman_->GetResource("OrbTextureAct");
	Resource *tex2 = resman_->GetResource("OrbTextureAlt");

	Orb *orb = new Orb("Orb", geom, mat, tex);
	orb->SetTextureAlt(tex2);
	orb->Scale(glm::vec3(1.75, 1.75, 1.75));
	orb->Translate(glm::vec3(0.0, 8.5, 0.0));
	orb->setLaserSpeed(rand() % 5 + 5);
	orb->setFireError(25);
	orb->setFireSpeed(10);
	orb->SetLaserDamage(25);

	twr->addChild(orb);

	towers_.push_back(twr);
	orbs_.push_back(orb);
	return twr;
}
            
} // namespace game
