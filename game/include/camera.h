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
    view_.setSize({kWindowWidth, kWindowHeight});

    UpdateBounds();
  }

  void UpdateBounds();

  sf::View& view(){return view_;}
};
}


#endif //MICROMACHINE_GAME_INCLUDE_CAMERA_H_