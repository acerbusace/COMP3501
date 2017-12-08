#include <stdexcept>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "tank.h"
#include "laser.h"

namespace game {


Tank::Tank(std::string name, Resource *geometry, Resource *material, Resource *texture) : SceneNode(name, geometry, material, texture) {
	bomb_timer_ = 15;
	bomb_speed_ = 1.0;
	fire_speed_ = 10.0;

	move_speed_ = 10;
	move_error_ = 20;

	bomb_damage_ = 5;
	body_damage_ = 25;
	health_ = 100;

	move_ = true;
}


Tank::~Tank() {
}


void Tank::Update(double delta_time) {
	fire_cooldown_ -= delta_time;

	//std::cout << "pos: " << position_.x << ", " << position_.y << ", " << position_.z << std::endl;
	position_ += velocity * (float)delta_time;
	//std::cout << "\tpos: " << position_.x << ", " << position_.y << ", " << position_.z << std::endl;
	//position_ += velocity;

	if (!move_) {
		if (glm::length(dest_ - position_) < move_speed_ * 2) {
			//std::cout << "have reached position!!!" << std::endl;
			velocity = glm::vec3(0, 0, 0);
			move_ = true;
		}
	}
}


float Tank::getFireSpeed() {
	return fire_speed_;
}


float Tank::getBombSpeed() {
	return bomb_speed_;
}


float Tank::getBombTimer() {
	return bomb_timer_;
}


float Tank::GetBombDamage() {
	return bomb_damage_;
}


float Tank::GetBodyDamage() {
	return body_damage_;
}


float Tank::getMoveError() {
	return move_error_;
}


float Tank::GetRadius() {
	return 4.0 * fmax(fmax(scale_.x, scale_.y), scale_.z);
}


bool Tank::move() {
	return move_;
}


float Tank::moveError() {
	return move_error_*((float)rand() / RAND_MAX) - move_error_ / 2.0;
}


bool Tank::shoot() {
	if (fire_cooldown_ < 0) {
		fireCoolDown();
		return true;
	}
	return false;
}


void Tank::fireCoolDown() {
	fire_cooldown_ = fire_speed_;
}


bool Tank::takeDamage(float damage) {
	health_ -= damage;

	std::cout << "tank health: " << health_ << std::endl;

	if (health_ <= 0)
		return true;
	return false;
}


void Tank::setFireSpeed(float speed) {
	fire_speed_ = speed;
}


void Tank::setBombSpeed(float speed) {
	bomb_speed_ = speed;
}


void Tank::setBombTimer(float timer) {
	bomb_timer_ = timer;
}


void Tank::SetBombDamage(float damage) {
	bomb_damage_ = damage;
}


void Tank::setMove(bool move) {
	move_ = move;
}


void Tank::setMoveError(float error) {
	move_error_ = error;
}


void Tank::setDestination(glm::vec2 destination) {
	dest_ = glm::vec3(destination.x, position_.y, destination.y);

	glm::vec3 direction = dest_ - position_;
	float dest_angle_ = glm::atan(direction.z / direction.x);

	velocity.x = glm::cos(dest_angle_) * move_speed_;
	if ((direction.x < 0 && velocity.x > 0) || (direction.x > 0 && velocity.x < 0))
		velocity.x *= -1;
	velocity.z = glm::sin(dest_angle_) * move_speed_;
	if ((direction.z < 0 && velocity.z > 0) || (direction.z > 0 && velocity.z < 0))
		velocity.z *= -1;
	move_ = false;
}

            
} // namespace game
