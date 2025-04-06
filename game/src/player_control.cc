#include "player_control.h"

namespace micromachine::player {

void Car::Tick(const float delta_time) {
  const float max_limit = 20.0f;
  const float min_limit = -20.0f;

  if(delta_time > 0)
  {
    velocity_ += velocity_ * delta_time;

    if(velocity_.x < min_limit)
    {
      velocity_.x = min_limit;
    }
    if(velocity_.y < min_limit)
    {
      velocity_.y = min_limit;
    }
    if(velocity_.x > max_limit)
    {
      velocity_.x = max_limit;
    }
    if(velocity_.y > max_limit)
    {
      velocity_.y = max_limit;
    }

    position_ += velocity_;

    velocity_ /= delta_time;
  }
}
}