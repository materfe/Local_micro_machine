//
// Created by Mat on 5/13/2025.
//

#ifndef MICROMACHINE_GAME_INCLUDE_CHECK_H_
#define MICROMACHINE_GAME_INCLUDE_CHECK_H_

#include "SFML/Graphics/RectangleShape.hpp"

//kept this way, potential adds in the future

namespace micromachine
{
class Checkpoint
{
 private:
  sf::RectangleShape shape_;
  float angle_deg = 0.0f;
 public:
  Checkpoint() = default;

  explicit Checkpoint(sf::Vector2f pos, float angle)
  {
    shape_.setPosition(pos);
    shape_.setSize({200.0f, 50.0f});
    shape_.setOrigin(shape_.getSize() / 2.0f);
    shape_.setFillColor(sf::Color::Yellow);
    angle_deg = angle;
    shape_.setRotation(sf::degrees(angle_deg));
  }

  sf::RectangleShape& shape(){return shape_;}
};

}



#endif //MICROMACHINE_GAME_INCLUDE_CHECK_H_
