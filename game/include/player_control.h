//
// Created by Mat on 3/20/2025.
//

#ifndef MICROMACHINE_INCLUDE_PLAYER_CONTROL_H_
#define MICROMACHINE_INCLUDE_PLAYER_CONTROL_H_

#include <SFML/System.hpp>
#include "game_state.h"
#include <SFML/Graphics.hpp>

namespace micromachine::player {

class Car {
 private:
  crackitos_physics::physics::PhysicsWorld& world_;
  crackitos_physics::physics::BodyHandle body_{};
  sf::RectangleShape shape_{};
  crackitos_physics::physics::ColliderHandle collider_{};
  crackitos_core::commons::fp speed_ = 0.0f;

  void ShapeUpdate()
  {
    shape_.setPosition({world_.GetMutableBody(body_).position().x, world_.GetMutableBody(body_).position().y});
  }

 public:
  Car(GameState &game_state,
      const crackitos_core::math::Vec2f &pos,
      const crackitos_core::commons::fp mass,
      const crackitos_core::commons::fp bounciness,
      const float speed = 40.0f) : world_(game_state.World()), speed_(speed){
    Init(game_state, pos, mass, bounciness);
  }

  void Init(GameState &game_state,
            const crackitos_core::math::Vec2f &pos,
            const crackitos_core::commons::fp mass,
            const crackitos_core::commons::fp bounciness){

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

    SetCarDamping(0.9999f);
  }

  bool operator==(const Car& car)
  {
    return this->body_ == car.body_ and this->collider_== car.collider_;
  }

  void Update(float dt)
  {
    crackitos_physics::physics::Body& body = world_.GetMutableBody(body_);

    body.Update(dt);
    ShapeUpdate();
  }

  void Move(crackitos_core::math::Vec2f direction)
  {
    crackitos_physics::physics::Body& body = world_.GetMutableBody(body_);
    body.ApplyForce(crackitos_core::math::Vec2f((direction * speed_).x, (direction * speed_).y));
  }

  void SetCarDamping(crackitos_core::commons::fp factor)
  {
    crackitos_physics::physics::Body& body = world_.GetMutableBody(body_);
    body.damping_factor(factor);
  }

  void SetColor(const sf::Color color)
  {
    shape_.setFillColor(color);
  }

  sf::RectangleShape& Shape(){return shape_;}
};
}

#endif //MICROMACHINE_INCLUDE_PLAYER_CONTROL_H_
