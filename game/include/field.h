//
// Created by Mat on 4/2/2025.
//

#ifndef MICROMACHINE_GAME_INCLUDE_FIELD_H_
#define MICROMACHINE_GAME_INCLUDE_FIELD_H_
#include <vector>
#include "SFML/Graphics.hpp"
#include "SFML/Graphics/Drawable.hpp."

#include "const.h"

namespace micromachine::field {

static constexpr std::int8_t TILE_SIZE = 16;
static constexpr std::int8_t GRID_WIDTH = 20;
static constexpr std::int8_t GRID_HEIGHT = 20;
static constexpr std::int8_t GENERATION_NUMBER = 25;

enum class TileType {
  Grass,
  Road
};

class Tile {
 private:
  sf::RectangleShape shape_;
  TileType type_ = TileType::Grass;
  sf::Texture tex_;

  void ApplyTextToShape() { shape_.setTexture(&tex_); }

 public:
  explicit Tile(sf::Vector2i pos) {
    shape_.setPosition(static_cast<sf::Vector2f>(pos));
    shape_.setFillColor(sf::Color::White);
    shape_.setOutlineThickness(-1.0f);
    shape_.setOutlineColor(sf::Color::Red);
    shape_.setSize({TILE_SIZE, TILE_SIZE});
  }

  void SetType(TileType type) { type_ = type; }
  TileType type() { return type_; }
  sf::RectangleShape &Shape() { return shape_; }
  void SetTextureTo(sf::Texture &texture) {
    tex_ = texture;
    ApplyTextToShape();
  }
};

class Tilemap {
 private:
  std::vector<Tile> map_{};

  void InitializeMap();
  static sf::Vector2<int> &RandomPathUpdate(sf::Vector2<int> &current_pos, int rnd);
  void SetAllTextures();

 public:
  Tilemap() {
    map_.reserve(GRID_WIDTH * GRID_HEIGHT);
  }

  void GenerateRandomMap();

  std::vector<Tile> &Map() { return map_; }
};

}

#endif //MICROMACHINE_GAME_INCLUDE_FIELD_H_
