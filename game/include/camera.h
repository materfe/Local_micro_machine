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

 public:
  Camera() {
    view_.setSize({WINDOW_WIDTH, WINDOW_HEIGHT});
  }


  void Update(micromachine::car_game_manager::Manager& manager)
  {
    auto position = sf::Vector2f(2000.0f, 2000.0f);

    for(auto& car : manager.AllPlayers())
    {
      const auto x = car.Shape().getPosition().x;
      const auto y = car.Shape().getPosition().y;
      if(x < position.x and y < position.y)
      {
        position = sf::Vector2f(x, y);
      }
    }
    view_.setCenter(position);
  }

  sf::View& view(){return view_;}
};
}


#endif //MICROMACHINE_GAME_INCLUDE_CAMERA_H_