#include "car_game_manager.h"

namespace micromachine::car_game_manager {
void Manager::TicksAll(const float delta_time) {
  for (auto &car : all_players_) {
    car->Update(delta_time);
  }
}

void Manager::AddPlayer(const player::Car &player) {
  all_players_.push_back(std::make_unique<player::Car>(player));
}

void Manager::RemovePlayer(player::Car &car) {
  auto it = std::find(all_players_.begin(), all_players_.end(), std::make_unique<player::Car>(car));
  if (it != all_players_.end()) {
    it->get()->SetColor(sf::Color::Black);
    it->get()->Shape().setOutlineThickness(-1.0f);
    it->get()->Shape().setOutlineColor(sf::Color::White);
    amount_of_players_--;
  }
}

}