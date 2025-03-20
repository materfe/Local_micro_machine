#include "playground.h"
#include <iostream>

namespace power_of_four {

void Playground::InsertCoin(TileState player, int col) {
  if (col < 0 || col >= kNumberOfColumns) {
    throw std::runtime_error("Invalid column index: too high or low");
  }

  // Start from the bottom row and find the first empty spot
  for (int row = kNumberOfRows - 1; row >= 0; row--) {
    int index = row * kNumberOfColumns + col; // Get correct index
    if (game_tiles_[index] == TileState::Empty) {
      game_tiles_[index] = player; // Place the coin here
      return;
    }
  }
}

TileState Playground::CheckColumns(const int amount_to_win) {
  // Check columns
  for (int col = 0; col < kNumberOfColumns; col++) {
    int player1_count = 0, player2_count = 0, player3_count = 0, player4_count = 0;
    for (int row = 0; row < kNumberOfRows; row++) {
      int index = col + (row * kNumberOfColumns);
      if (index < 0 || index >= kNumberOfColumns * kNumberOfRows) {
        break;  // Prevents accessing invalid memory
      }
      if (game_tiles_[index] == TileState::Player1) {
        player1_count++;
        player2_count = 0;
        player3_count = 0;
        player4_count = 0;
      } else if (game_tiles_[index] == TileState::Player2) {
        player1_count = 0;
        player2_count++;
        player3_count = 0;
        player4_count = 0;
      } else if (game_tiles_[index] == TileState::Player3) {
        player1_count = 0;
        player2_count = 0;
        player3_count++;
        player4_count = 0;
      } else if (game_tiles_[index] == TileState::Player4) {
        player1_count = 0;
        player2_count = 0;
        player3_count = 0;
        player4_count++;
      } else {
        player1_count = 0;
        player2_count = 0;
        player3_count = 0;
        player4_count = 0;
      }
      if (player1_count == amount_to_win) {
        std::cout << "column checking true for player 1\n";
        return TileState::Player1;
      }
      if (player2_count == amount_to_win) {
        std::cout << "column checking true for player 2\n";
        return TileState::Player2;
      }
      if (player3_count == amount_to_win) {
        std::cout << "column checking true for player 3\n";
        return TileState::Player3;
      }
      if (player4_count == amount_to_win) {
        std::cout << "column checking true for player 4\n";
        return TileState::Player4;
      }
    }
  }
  return TileState::Empty;
}

TileState Playground::CheckRows(const int amount_to_win) {
  for (int row = 0; row < kNumberOfRows; row++) {
    int player1_count = 0, player2_count = 0, player3_count = 0, player4_count = 0;
    for (int col = 0; col < kNumberOfColumns; col++) {
      int index = row * kNumberOfColumns + col;
      if (index < 0 || index >= kNumberOfColumns * kNumberOfRows) {
        break;  // Prevents accessing invalid memory
      }
      if (game_tiles_[index] == TileState::Player1) {
        player1_count++;
        player2_count = 0;
        player3_count = 0;
        player4_count = 0;
      } else if (game_tiles_[index] == TileState::Player2) {
        player1_count = 0;
        player2_count++;
        player3_count = 0;
        player4_count = 0;
      } else if (game_tiles_[index] == TileState::Player3) {
        player1_count = 0;
        player2_count = 0;
        player3_count++;
        player4_count = 0;
      } else if (game_tiles_[index] == TileState::Player4) {
        player1_count = 0;
        player2_count = 0;
        player3_count = 0;
        player4_count++;
      } else {
        player1_count = 0;
        player2_count = 0;
        player3_count = 0;
        player4_count = 0;
      }
      if (player1_count == amount_to_win) {
        std::cout << "column checking true for player 1\n";
        return TileState::Player1;
      }
      if (player2_count == amount_to_win) {
        std::cout << "column checking true for player 2\n";
        return TileState::Player2;
      }
      if (player3_count == amount_to_win) {
        std::cout << "column checking true for player 3\n";
        return TileState::Player3;
      }
      if (player4_count == amount_to_win) {
        std::cout << "column checking true for player 4\n";
        return TileState::Player4;
      }
    }
  }

  return TileState::Empty;
}

TileState Playground::CheckDiagonal1(const int amount_to_win) {
  for (int row = 0; row <= kNumberOfRows - amount_to_win; row++) {
    for (int col = 0; col <= kNumberOfColumns - amount_to_win; col++) {
      int player1_count = 0, player2_count = 0, player3_count = 0, player4_count = 0;
      for (int i = 0; i < amount_to_win; i++) {
        int index = (row + i) * kNumberOfColumns + (col + i);
        if (index < 0 || index >= kNumberOfColumns * kNumberOfRows) {
          break;  // Prevents accessing invalid memory
        }
        if (game_tiles_[index] == TileState::Player1) {
          player1_count++;
          player2_count = 0;
          player3_count = 0;
          player4_count = 0;
        } else if (game_tiles_[index] == TileState::Player2) {
          player1_count = 0;
          player2_count++;
          player3_count = 0;
          player4_count = 0;
        } else if (game_tiles_[index] == TileState::Player3) {
          player1_count = 0;
          player2_count = 0;
          player3_count++;
          player4_count = 0;
        } else if (game_tiles_[index] == TileState::Player4) {
          player1_count = 0;
          player2_count = 0;
          player3_count = 0;
          player4_count++;
        } else {
          player1_count = 0;
          player2_count = 0;
          player3_count = 0;
          player4_count = 0;
        }
        if (player1_count == amount_to_win) {
          std::cout << "column checking true for player 1\n";
          return TileState::Player1;
        }
        if (player2_count == amount_to_win) {
          std::cout << "column checking true for player 2\n";
          return TileState::Player2;
        }
        if (player3_count == amount_to_win) {
          std::cout << "column checking true for player 3\n";
          return TileState::Player3;
        }
        if (player4_count == amount_to_win) {
          std::cout << "column checking true for player 4\n";
          return TileState::Player4;
        }
      }
    }
  }
  return TileState::Empty;
}

TileState Playground::CheckDiagonal2(const int amount_to_win) {
  for (int row = 0; row <= kNumberOfRows - amount_to_win; row++) {
    for (int col = amount_to_win - 1; col < kNumberOfColumns; col++) {
      int player1_count = 0, player2_count = 0, player3_count = 0, player4_count = 0;
      for (int i = 0; i < amount_to_win; i++) {
        int index = (row + i) * kNumberOfColumns + (col - i);
        if (index < 0 || index >= kNumberOfColumns * kNumberOfRows) {
          break;  // Prevents accessing invalid memory
        }
        if (game_tiles_[index] == TileState::Player1) {
          player1_count++;
          player2_count = 0;
          player3_count = 0;
          player4_count = 0;
        } else if (game_tiles_[index] == TileState::Player2) {
          player1_count = 0;
          player2_count++;
          player3_count = 0;
          player4_count = 0;
        } else if (game_tiles_[index] == TileState::Player3) {
          player1_count = 0;
          player2_count = 0;
          player3_count++;
          player4_count = 0;
        } else if (game_tiles_[index] == TileState::Player4) {
          player1_count = 0;
          player2_count = 0;
          player3_count = 0;
          player4_count++;
        } else {
          player1_count = 0;
          player2_count = 0;
          player3_count = 0;
          player4_count = 0;
        }
        if (player1_count == amount_to_win) {
          std::cout << "column checking true for player 1\n";
          return TileState::Player1;
        }
        if (player2_count == amount_to_win) {
          std::cout << "column checking true for player 2\n";
          return TileState::Player2;
        }
        if (player3_count == amount_to_win) {
          std::cout << "column checking true for player 3\n";
          return TileState::Player3;
        }
        if (player4_count == amount_to_win) {
          std::cout << "column checking true for player 4\n";
          return TileState::Player4;
        }
      }
    }
  }
  return TileState::Empty;
}

TileState Playground::CheckIfAnyPlayersWins() {

  const std::int32_t amount_to_win = 4;

  auto result = CheckColumns(amount_to_win);

  if(result != TileState::Empty)
  {
    return result;
  }

  result = CheckRows(amount_to_win);
  if(result != TileState::Empty)
  {
    return result;
  }

// Check diagonal (\ direction)
  result = CheckDiagonal1(amount_to_win);
  if(result != TileState::Empty)
  {
    return result;
  }

// Check diagonal (/ direction)
  result = CheckDiagonal2(amount_to_win);
  if(result != TileState::Empty)
  {
    return result;
  }

  return TileState::Empty;  // No win detected
}

TileState Playground::ReturnState(int index) {

  if (index < game_tiles_.size() and index >= 0) {
    return game_tiles_[index];
  }
  return TileState::Empty;
}

} //namespace power_of_four