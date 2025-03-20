#include "player_control.h"

namespace micromachine::player {
void Car::Tick(const float delta_time) {
  position_.x += velocity_.x / delta_time;
  position_.y += velocity_.y / delta_time;
  velocity_.x -= delta_time;
  velocity_.y -= delta_time;
}

}