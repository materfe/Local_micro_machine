//
// Created by Mat on 4/21/2025.
//

#ifndef MICROMACHINE_GAME_INCLUDE_CAMERA_H_
#define MICROMACHINE_GAME_INCLUDE_CAMERA_H_

#include "SFML/Graphics.hpp"
#include "car_game_manager.h"
#include "const.h"

namespace micromachine::View{

class Camera
{
 private:
  sf::View view_{};
  crackitos_core::math::AABB bounds_;


 public:
  Camera() {
    view_.setSize({WINDOW_WIDTH, WINDOW_HEIGHT});

    UpdateBounds();
  }

  void UpdateBounds()
  {
    const auto half_width = view_.getSize().x / 2;
    const auto half_height = view_.getSize().y / 2;

    crackitos_core::math::Vec2f min_bounds(view_.getCenter().x - half_width,
                                           view_.getCenter().y - half_height);
    crackitos_core::math::Vec2f max_bounds(view_.getCenter().x + half_width,
                                           view_.getCenter().y + half_height);

    bounds_.set_min_bound(min_bounds);
    bounds_.set_max_bound(max_bounds);
  }
  void Update(micromachine::car_game_manager::Manager& manager)
  {
    auto position = sf::Vector2f(2000.0f, 2000.0f);

    for(auto& car : manager.AllPlayers())
    {
      const auto x = car.Shape().getPosition().x;
      const auto y = car.Shape().getPosition().y;
      if(y < position.y)
      {
        position = sf::Vector2f(x, y);
      }
    }
    view_.setCenter(position);
    UpdateBounds();
  }

  bool IsInPlayableBounds(player::Car& car)
  {
    const auto half_width = car.Shape().getSize().x / 2;
    const auto half_height = car.Shape().getSize().y / 2;

    const auto car_aabb = crackitos_core::math::AABB({car.Shape().getPosition().x - half_width,
                                                     car.Shape().getPosition().y - half_height},
                                                     {car.Shape().getPosition().x + half_width,
                                                      car.Shape().getPosition().y + half_height});
    return Intersect(bounds_, car_aabb);
  }

  sf::View& view(){return view_;}
};
}


#endif //MICROMACHINE_GAME_INCLUDE_CAMERA_H_