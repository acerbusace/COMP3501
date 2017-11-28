#include <stdexcept>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "tank.h"
#include "laser.h"
//#include "misc.h"

namespace game {

	//extern ResourceManager resman;

Tank::Tank(std::string name, Resource *geometry, Resource *material, Resource *texture) : SceneNode(name, geometry, material, texture) {
	fire_error_ = 25.0;
	laser_speed_ = 5.0;
	fire_speed_ = 10.0;

	move_speed_ = 2.5;
	move_error_ = 20;

	curr_angle_ = 0;
	dest_angle_ = curr_angle_;
	turning_speed_ = 0.1;

	move_ = true;
}


Tank::~Tank(){
}

void Tank::Update(double delta_time){
	fire_cooldown_ -= delta_time;

	//std::cout << "pos: " << position_.x << ", " << position_.y << ", " << position_.z << std::endl;
	position_ += velocity * (float) delta_time;
	//std::cout << "\tpos: " << position_.x << ", " << position_.y << ", " << position_.z << std::endl;
	//position_ += velocity;

	float turning_speed = turning_speed_ * delta_time;
	if (curr_angle_ + turning_speed < dest_angle_)
		curr_angle_ += turning_speed;
	else if (curr_angle_ - turning_speed > dest_angle_)
		curr_angle_ -= turning_speed;
	else {
		curr_angle_ = dest_angle_;
		//std::cout << "have set angle!!!" << std::endl;
	}

	SetOrientation(glm::angleAxis(curr_angle_, glm::vec3(0, 1, 0)));


	if (!move_) {
		if (glm::length(dest_ - position_) < move_speed_*2) {
			std::cout << "have reached position!!!" << std::endl;
			velocity = glm::vec3(0, 0, 0);
			move_ = true;
		}
	}
}

void Tank::setDestination(glm::vec2 destination) {
	dest_ = glm::vec3(destination.x, position_.y, destination.y);

	glm::vec3 direction = dest_ - position_;
	dest_angle_ = glm::atan(direction.z / direction.x);

	//velocity.x = direction.x / (direction.x + direction.z) * move_speed_;
	//velocity.z = direction.z / (direction.x + direction.z) * move_speed_;

	velocity.x = glm::cos(dest_angle_) * move_speed_;
	if ((direction.x < 0 && velocity.x > 0) || (direction.x > 0 && velocity.x < 0))
		velocity.x *= -1;
	velocity.z = glm::sin(dest_angle_) * move_speed_;
	if ((direction.z < 0 && velocity.z > 0) || (direction.z > 0 && velocity.z < 0))
		velocity.z *= -1;

	std::cout << "x: " << position_.x << " y: " << position_.y << " z: " << position_.z << std::endl;
	std::cout << "\tx: " << direction.x << " y: " << direction.y << " z: " << direction.z << std::endl;
	std::cout << "\t\tx: " << velocity.x << " y: " << velocity.y << " z: " << velocity.z << std::endl;
	std::cout << "\t\tangle: " << dest_angle_ << std::endl;
	move_ = false;
}

bool Tank::move() {
	return move_;
}

void Tank::setMove(bool move) {
	move_ = move;
}

float Tank::fireError() {
	return fire_error_*((float) rand()/RAND_MAX) - fire_error_ / 2.0;
}

float Tank::moveError() {
	return move_error_*((float) rand()/RAND_MAX) - move_error_ / 2.0;
}

float Tank::getLaserSpeed() {
	return laser_speed_;
}

float Tank::getFireSpeed() {
	return fire_speed_;
}

float Tank::getMoveError() {
	return move_error_;
}

bool Tank::shoot() {
	if (fire_cooldown_ < 0) {
		fireCoolDown();
		return true;
	}
	return false;
}

void Tank::setLaserSpeed(float speed) {
	laser_speed_ = speed;
}

void Tank::setFireSpeed(float speed) {
	fire_speed_ = speed;
}

void Tank::setFireError(float error) {
	fire_error_ = error;
}

void Tank::setMoveError(float error) {
	move_error_ = error;
}

void Tank::fireCoolDown() {
	fire_cooldown_ = fire_speed_;
}
            
} // namespace game
