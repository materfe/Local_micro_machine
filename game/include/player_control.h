//
// Created by Mat on 3/20/2025.
//

#ifndef MICROMACHINE_INCLUDE_PLAYER_CONTROL_H_
#define MICROMACHINE_INCLUDE_PLAYER_CONTROL_H_

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <set>
#include "game_state.h"

namespace micromachine::player {

class Car {
 private:
  crackitos_physics::physics::PhysicsWorld &world_;
  crackitos_physics::physics::BodyHandle body_{};
  sf::RectangleShape shape_{};
  sf::Texture text_; //forced to keep this otherwise get an error because texture destroyed
  crackitos_physics::physics::ColliderHandle collider_{};
  crackitos_core::commons::fp speed_ = 0.0f;


  void ShapeUpdate() {
    shape_.setPosition({world_.GetMutableBody(body_).position().x, world_.GetMutableBody(body_).position().y});
  }

 public:
  Car(GameState &game_state,
      const crackitos_core::math::Vec2f &pos,
      const crackitos_core::commons::fp mass,
      const crackitos_core::commons::fp bounciness,
      const float speed = 40.0f) : world_(game_state.World()), speed_(speed) {
    Init(game_state, pos, mass, bounciness);
  }

  void Init(GameState &game_state,
            const crackitos_core::math::Vec2f &pos,
            crackitos_core::commons::fp mass,
            crackitos_core::commons::fp bounciness);

  bool operator==(const Car &car) {
    return this->body_ == car.body_ and this->collider_ == car.collider_;
  }

  void Update(float dt) {
    crackitos_physics::physics::Body &body = world_.GetMutableBody(body_);

    body.Update(dt);
    ShapeUpdate();
  }

  void Move(crackitos_core::math::Vec2f direction) {
    crackitos_physics::physics::Body &body = world_.GetMutableBody(body_);
    body.ApplyForce(crackitos_core::math::Vec2f((direction * speed_).x, (direction * speed_).y));
  }

  void SetCarDamping(crackitos_core::commons::fp factor) {
    crackitos_physics::physics::Body &body = world_.GetMutableBody(body_);
    body.damping_factor(factor);
  }

  void SetColor(const sf::Color color) {
    shape_.setFillColor(color);
  }

  void SetStartPos(const crackitos_core::math::Vec2f &start_pos) {
    crackitos_physics::physics::Body &body = world_.GetMutableBody(body_);
    body.set_position(start_pos);
    ShapeUpdate();
  }

  sf::RectangleShape &Shape() { return shape_; }
};
}

#endif //MICROMACHINE_INCLUDE_PLAYER_CONTROL_H_
