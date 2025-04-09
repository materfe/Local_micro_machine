//
// Created by Mat on 3/20/2025.
//

#ifndef MICROMACHINE_INCLUDE_PLAYER_CONTROL_H_
#define MICROMACHINE_INCLUDE_PLAYER_CONTROL_H_

#include <SFML/System.hpp>
namespace micromachine::player {

struct CarOption
{
  bool is_accelerating = false;
  bool is_turning_left = false;
  bool is_turning_right = false;
  bool is_braking = false;
  float angle = 0.0f; //degree in here, rad in the main

  void TurnAccelerationTo(bool value){is_accelerating = value;}
  void TurnTurningLeftTo(bool value){is_turning_left = value;}
  void TurnTurningRightTo(bool value){is_turning_right = value;}
  void TurnBrakingTo(bool value){is_braking = value;}
};

class Car {
 private:
  sf::Vector2<float> position_{};
  sf::Vector2<float> velocity_{};
  CarOption option_;

 public:
  explicit Car(sf::Vector2<float> new_position = sf::Vector2f(0.0f, 0.0f),
               sf::Vector2<float> new_velocity = sf::Vector2f(0.0f, 0.0f)) : position_(
      new_position), velocity_(new_velocity) {};

  void Tick(float delta_time);
  void Test(float delta_time);

  //set values
  void MoveTo(sf::Vector2f new_position) { position_ += new_position; }
  void MoveAt(sf::Vector2f new_velocity) { velocity_ += new_velocity; }

  sf::Vector2f Position() { return position_; }
  sf::Vector2f Velocity() { return velocity_; }
  CarOption& Options() {return option_;}
};
}

#endif //MICROMACHINE_INCLUDE_PLAYER_CONTROL_H_
