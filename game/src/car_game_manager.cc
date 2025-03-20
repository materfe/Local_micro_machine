#include "car_game_manager.h"

namespace micromachine::car_game_manager {
void Manager::AllTicks(const float delta_time) {
  for(auto& car : all_players_)
  {
    car.Tick(delta_time);
  }
}

void Manager::AddPlayer(const player::Car& player) {
  all_players_.push_back(player);
}
}