#include "car_game_manager.h"

namespace micromachine::car_game_manager {
void Manager::TicksAll(const float delta_time) {
  for(auto& car : all_players_)
  {
    car.Update(delta_time);
  }
}

void Manager::AddPlayer(const player::Car& player) {
  all_players_.push_back(player);
}
}