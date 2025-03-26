//
// Created by Mat on 3/20/2025.
//

#ifndef MICROMACHINE_GAME_INCLUDE_CAR_GAME_MANAGER_H_
#define MICROMACHINE_GAME_INCLUDE_CAR_GAME_MANAGER_H_

#include <vector>
#include "player_control.h"

namespace micromachine::car_game_manager {

class Manager final {
 private:
  int amount_of_players_ = 0;
  std::vector<player::Car> all_players_{};

 public:
  explicit Manager(std::int32_t amount_of_players = 0) : amount_of_players_(amount_of_players) {
    all_players_.reserve(amount_of_players_);
  }


  std::vector<player::Car>& AllPlayers(){return all_players_;}

  void AllTicks(float delta_time);
  void AddPlayer(const player::Car& player);
};

}

#endif //MICROMACHINE_GAME_INCLUDE_CAR_GAME_MANAGER_H_
