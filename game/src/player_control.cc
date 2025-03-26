#include <iostream>
#include "player_control.h"

namespace micromachine::player {

void Car::Tick(const float delta_time) {
  if(delta_time > 0)
  {
    position_.x += velocity_.x / delta_time;
    position_.y += velocity_.y / delta_time;

    velocity_.x -= delta_time /2;
    velocity_.y -= delta_time /2;
    if(velocity_.x < 0.0f)
    {
      velocity_.x = 0.0;
    }
    if(velocity_.y < 0.0f)
    {
      velocity_.y = 0.0f;
    }
  }
}

}