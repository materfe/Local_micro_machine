#include "car_game_manager.h"

namespace micromachine::car_game_manager {
void Manager::TicksAll(const float delta_time) {
  for (auto &car : all_players_) {
    car.Update(delta_time);
  }
}

void Manager::AddPlayer(const player::Car &player) {
  all_players_.push_back(player);
}

void Manager::SetAllPositions(const crackitos_core::math::Vec2f &start_pos) {
  auto offset = 0.0f;

  for (auto &car : all_players_) {
    const crackitos_core::math::Vec2f pos(start_pos.x + offset, start_pos.y);
    car.SetStartPos(pos);
    offset += car.Shape().getSize().x;
  }
}
}