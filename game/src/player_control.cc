#include <iostream>
#include "player_control.h"

namespace micromachine::player {

void Car::Tick(const float delta_time) {
  if(delta_time > 0)
  {
    position_.x += velocity_.x / delta_time;
    position_.y += velocity_.y / delta_time;

    //velocity_.x -= delta_time /2;
    //sssvelocity_.y -= delta_time /2;
  }
}
}