//
// Created by Mat on 3/20/2025.
//

#ifndef MICROMACHINE_INCLUDE_PLAYER_CONTROL_H_
#define MICROMACHINE_INCLUDE_PLAYER_CONTROL_H_

#include <SFML/System.hpp>
#include "game_state.h"
#include <SFML/Graphics.hpp>

namespace micromachine::player {

class Cars {
 private:
  crackitos_physics::physics::BodyHandle body_{};
  sf::RectangleShape shape_{};
  crackitos_physics::physics::ColliderHandle collider_{};

 public:

  Cars(GameState &game_state,
       const crackitos_core::math::Vec2f &pos,
       const crackitos_core::commons::fp mass,
       const crackitos_core::commons::fp bounciness) {
    Init(game_state, pos, mass, bounciness);
  }

  void Init(GameState &game_state,
            const crackitos_core::math::Vec2f &pos,
            const crackitos_core::commons::fp mass,
            const crackitos_core::commons::fp bounciness) {

    const auto type = crackitos_physics::physics::BodyType::Dynamic;
    body_ = game_state.CreateBody(type, pos, mass);

    const auto car_bounds = crackitos_core::math::AABB({pos.x - 8, pos.y - 8}, {pos.x + 8, pos.y + 8});
    const auto is_trigger = false;
    collider_ = game_state.CreateCollider(body_, car_bounds, bounciness, is_trigger);

    const auto shape_size = sf::Vector2f(car_bounds.max_bound().x - car_bounds.min_bound().x,
                                         car_bounds.max_bound().y - car_bounds.min_bound().y);
    shape_.setPosition({car_bounds.GetCentre().x, car_bounds.GetCentre().y});
    shape_.setSize(shape_size);
    shape_.setFillColor(sf::Color::Magenta);
  }

  sf::RectangleShape& Shape(){return shape_;}
};

struct CarOption {
  bool is_accelerating = false;
  bool is_turning_left = false;
  bool is_turning_right = false;
  bool is_braking = false;
  float angle = 0.0f; //degree in here, rad in the main

  void TurnAccelerationTo(bool value) { is_accelerating = value; }
  void TurnTurningLeftTo(bool value) { is_turning_left = value; }
  void TurnTurningRightTo(bool value) { is_turning_right = value; }
  void TurnBrakingTo(bool value) { is_braking = value; }
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

  sf::Vector2f Position() { return position_; }
  CarOption &Options() { return option_; }
};
}

#endif //MICROMACHINE_INCLUDE_PLAYER_CONTROL_H_
