//
// Created by Mat on 2/26/2025.
//

#ifndef SIMPLECHAT_PLAYGROUND_H
#define SIMPLECHAT_PLAYGROUND_H

#include <array>

namespace power_of_four {
static constexpr std::int32_t kNumberOfRows = 10;
static constexpr std::int32_t kNumberOfColumns = 13;

enum class TileState {
  Empty,
  Player1,
  Player2,
  Player3,
  Player4
};

class Playground {
 private :
  std::array<TileState, kNumberOfRows * kNumberOfColumns> game_tiles_{};

  TileState CheckColumns(int amount_to_win);
  TileState CheckRows(int amount_to_win);
  TileState CheckDiagonal1(int amount_to_win);
  TileState CheckDiagonal2(int amount_to_win);
 public:
  Playground() {
    for (auto &tile : game_tiles_) {
      tile = TileState::Empty;
    }
  }

  //set tiles
  void InsertCoin(TileState player, int col);

  TileState ReturnState(int index);

  TileState CheckIfAnyPlayersWins();


};
}

#endif //SIMPLECHAT_PLAYGROUND_H
