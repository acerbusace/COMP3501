#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "tower_control.h"
#include "misc.h"

namespace game {

TowerControl::TowerControl(ResourceManager *rm) : resman_(rm) { }

TowerControl::~TowerControl(){
}

void TowerControl::update(double delta_time, glm::vec3 player_pos){
	//player_pos = glm::vec3(10.0, 0.0, 11.0);

	for each (Orb *orb in orbs_) {
		orb->Update(delta_time);

		if (orb->shoot())
			shoot(orb, player_pos);
	}

	//for each (Laser *lsr in lasers_) {
	for (int i = 0; i < lasers_.size(); ++i) {
		lasers_[i]->Update(delta_time);

		if (lasers_[i]->done()) {
			lasers_.erase(lasers_.begin() + i);
			//std::cout << "deleting laser" << std::endl;
		}
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
	lasers_.push_back(lsr);
}

void TowerControl::init() {
	for (int i = 0; i < 25; ++i) {
		createTowerInstance(glm::vec3(rand() % 1000 - 500, 0, rand() % 1000 - 500));
	}
}

void TowerControl::draw(Camera *camera) {
	for each (Tower *twr in towers_) {
		twr->Draw(camera);
	}

	for each (Laser *lsr in lasers_) {
		lsr->Draw(camera);
	}
}

Tower *TowerControl::createTowerInstance(glm::vec3 pos) {
	Resource *geom = getResource(resman_, "TowerMesh");
	Resource *mat = getResource(resman_, "ShinyBlueMaterial");
	Resource *tex = resman_->GetResource("");

	Tower *twr = new Tower("Tower", geom, mat, tex);
	twr->Scale(glm::vec3(1.5, 3.0, 1.5));
	twr->Translate(pos + glm::vec3(0.0, 0.0, 0.0));

	geom = getResource(resman_, "SphereMesh");
	mat = getResource(resman_, "ShinyTextureMaterial");
	tex = resman_->GetResource("");

	Orb *orb = new Orb("Orb", geom, mat, tex);
	orb->Scale(glm::vec3(1.75, 1.75, 1.75));
	orb->Translate(glm::vec3(0.0, 8.5, 0.0));
	orb->setLaserSpeed(rand() % 5 + 5);
	orb->setFireError(25);
	orb->setFireSpeed(10);

	twr->addChild(orb);

	towers_.push_back(twr);
	orbs_.push_back(orb);
	return twr;
}
            
} // namespace game
