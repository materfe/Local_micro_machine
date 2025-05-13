#include <iostream>
#include "player_control.h"

namespace micromachine::player {
void Car::Init(micromachine::GameState &game_state,
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

  std::string_view path("data/Cars/car_black_small_3.png");
  if(!text_.loadFromFile(path)){
    std::cerr << "big problems for car\n";
  }
  shape_.setTexture(&text_);


  SetCarDamping(0.9999f);
}


}