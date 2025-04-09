#include <iostream>
#include "player_control.h"

namespace micromachine::player {

void Car::Tick(const float delta_time) {
  const float max_limit = 200.0f;
  const float min_limit = -200.0f;
  const float friction = 40.0f;

  const auto friction_effect = friction * delta_time;

  if (velocity_.x > 0.0f) {
    velocity_.x -= friction_effect;
  }
  if (velocity_.x < 0.0f) {
    velocity_.x += friction_effect;
  }
  if (velocity_.y < 0.0f) {
    velocity_.y += friction_effect;
  }
  if (velocity_.y > 0.0f) {
    velocity_.y -= friction_effect;
  }

  //clamp limits
  velocity_.x = std::clamp(velocity_.x, min_limit, max_limit);
  velocity_.y = std::clamp(velocity_.y, min_limit, max_limit);

  //move
  position_ += velocity_ * delta_time;

  //std::cout << "velo : [ " << velocity_.x << " , " << velocity_.y << " ] " << "\n";
}

void Car::Test(const float delta_time) {
  const float max_speed = 200.0f;
  const float acceleration = 300.0f;
  const float friction = 200.0f;
  const float turn_speed = 2.5f;

  //rotation setup
  if (option_.is_turning_left) {
    option_.angle -= turn_speed * delta_time;
  }
  if (option_.is_turning_right) {
    option_.angle += turn_speed * delta_time;
  }

  //acceleration
  if (option_.is_accelerating) {
    velocity_ += sf::Vector2f(std::cos(option_.angle), std::sin(option_.angle)) * acceleration * delta_time;
  }

  //if braking
  if (option_.is_braking) {
    velocity_ -= sf::Vector2f(std::cos(option_.angle), std::sin(option_.angle)) * acceleration * delta_time;
  }

  //friction
  if (velocity_.length() > 0.0f) {
    sf::Vector2f friction_force = velocity_.normalized() * -friction * delta_time;
    if (friction_force.length() > velocity_.length()) {
      velocity_ = sf::Vector2f(0.0f, 0.0f);
    } else {
      velocity_ += friction_force;
    }
  }

  // Clamp la vitesse max
  if (velocity_.length() > max_speed) {
    velocity_ = velocity_.normalized() * max_speed;
  }

  // Mise à jour de la position
  position_ += velocity_ * delta_time;

//  option_.TurnAccelerationTo(false);
//  option_.TurnTurningRightTo(false);
//  option_.TurnTurningLeftTo(false);
//  option_.TurnBrakingTo(false);
}
}