//
// Created by Mat on 3/20/2025.
//

#ifndef MICROMACHINE_INCLUDE_PLAYER_CONTROL_H_
#define MICROMACHINE_INCLUDE_PLAYER_CONTROL_H_

#include <SFML/System.hpp>
namespace micromachine::player {

class Car {
 private:
  sf::Vector2<float> position_{};
  sf::Vector2<float> velocity_{};

 public:

  explicit Car(sf::Vector2<float> new_position = sf::Vector2f(0.0f, 0.0f),
               sf::Vector2<float> new_velocity = sf::Vector2f(0.0f, 0.0f)) : position_(
      new_position), velocity_(new_velocity) {};

  void Tick(float delta_time);

  //set values
  void MoveTo(sf::Vector2f new_position) { position_ = new_position; }
  void MoveAt(sf::Vector2f new_velocity) { velocity_ = new_velocity; }
  sf::Vector2f Position() { return position_; }
  sf::Vector2f Velocity() { return velocity_; }
};
}

#endif //MICROMACHINE_INCLUDE_PLAYER_CONTROL_H_
