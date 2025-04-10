#include <iostream>
#include "player_control.h"

namespace micromachine::player {

void Car::Tick(const float delta_time) {
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
//  w

  //max speed
  if (velocity_.length() > max_speed) {
    velocity_ = velocity_.normalized() * max_speed;
  }

  position_ += velocity_ * delta_time;

//  option_.TurnAccelerationTo(false);
//  option_.TurnTurningRightTo(false);
//  option_.TurnTurningLeftTo(false);
//  option_.TurnBrakingTo(false);
}
}