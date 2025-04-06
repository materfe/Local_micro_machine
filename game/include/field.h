//
// Created by Mat on 4/2/2025.
//

#ifndef MICROMACHINE_GAME_INCLUDE_FIELD_H_
#define MICROMACHINE_GAME_INCLUDE_FIELD_H_
#include <vector>
#include "SFML/Graphics.hpp"

#include "const.h"

namespace micromachine::field {

static constexpr std::int8_t TILE_SIZE = 32;
static constexpr std::int32_t GRID_WIDTH = WINDOW_WIDTH / TILE_SIZE;
static constexpr std::int32_t GRID_HEIGHT = WINDOW_HEIGHT / TILE_SIZE;
static constexpr std::int32_t GENERATION_AMOUNT = 25;

class Tile {
 private:
  sf::RectangleShape shape_{};
  sf::Vector2i position_{};
  sf::Texture texture_{};

  void SetShape() {
    shape_.setPosition(static_cast<sf::Vector2f>(position_));
    shape_.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    shape_.setOutlineThickness(-1.0f);
    shape_.setOutlineColor(sf::Color::White);
  }

 public:
  explicit Tile(sf::Vector2i pos, sf::Texture &text) : position_(pos), texture_(text) {
    SetShape();
  }
  explicit Tile(sf::Vector2i pos) : position_(pos) {
    SetShape();
  }

  [[nodiscard]] sf::Vector2i Position() { return position_; }
  [[nodiscard]] sf::RectangleShape &Shape() { return shape_; }
};

class Tilemap {
 private:
  std::vector<Tile> map_{};
  std::array<bool, GRID_WIDTH*GRID_HEIGHT> visited_{false};

  [[nodiscard]] static int getIndex(int x, int y) {
    int index = y * GRID_WIDTH + x;
    if (index < 0 || index >= GRID_WIDTH * GRID_HEIGHT) {
      std::cerr << "Invalid index: " << index << " for pos (" << x << ", " << y << ")" << std::endl;
    }
    return index;
  }

  bool isVisited(int x, int y) {
    auto index = getIndex(x, y);
    return visited_[index];
  }

  void markVisited(int x, int y) {
    visited_[getIndex(x, y)] = true;
  }

  void FulfillMap();

  void FulfillVisited();

  void SetTextures();

 public:
  Tilemap() {
    map_.reserve(GRID_WIDTH * GRID_HEIGHT);
  }

  void GeneratePath();

  std::vector<Tile> &Map() { return map_; }
};

}

#endif //MICROMACHINE_GAME_INCLUDE_FIELD_H_
