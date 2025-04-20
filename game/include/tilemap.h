//
// Created by Mat on 4/2/2025.
//

#ifndef MICROMACHINE_GAME_INCLUDE_TILEMAP_H_
#define MICROMACHINE_GAME_INCLUDE_TILEMAP_H_
#include <vector>
#include "SFML/Graphics.hpp"
#include "SFML/Graphics/Drawable.hpp."

#include "const.h"
#include "commons.h"

namespace micromachine::tilemap {
static constexpr std::int8_t GRID_WIDTH = 20;
static constexpr std::int8_t GRID_HEIGHT = 20;
static constexpr std::int8_t GENERATION_NUMBER = 25;

enum class TileType {
  Grass,
  Road,
  StartRoad,
  EndRoad
};

class Tile {
 private:
  sf::RectangleShape shape_;
  TileType type_ = TileType::Grass;
  sf::Texture tex_;
  crackitos_core::commons::fp size_ = 0.0f;

  void ApplyTextToShape() { shape_.setTexture(&tex_); }

 public:
  explicit Tile(sf::Vector2i pos) {
    shape_.setPosition(static_cast<sf::Vector2f>(pos));
    shape_.setFillColor(sf::Color::White);
    shape_.setOutlineThickness(-1.0f);
    shape_.setOutlineColor(sf::Color::Red);
    SetSizeTo(16.0f);
  }

  Tile() = default;

  void SetType(TileType type) { type_ = type; }
  TileType type() { return type_; }
  sf::RectangleShape &Shape() { return shape_; }
  void SetTextureTo(sf::Texture &texture) {
    tex_ = texture;
    ApplyTextToShape();
  }
  void SetSizeTo(const crackitos_core::commons::fp &size) {
    size_ = size;
    shape_.setSize({size_, size_});
  }
};

class Tilemap {
 private:
  std::vector<Tile> map_{};
  std::vector<Tile> road_{};

  void InitializeMap();
  static sf::Vector2<int> &ChooseNeighbour(sf::Vector2<int> &current_pos, int rnd);
  void SetAllTextures();
  void SetStartAndEnd();

 public:
  Tilemap() {
    map_.reserve(GRID_WIDTH * GRID_HEIGHT);
    road_.reserve(GRID_WIDTH * GRID_HEIGHT);
  }

  void GenerateRandomMap();
  void SetAllTileSizeTo(crackitos_core::commons::fp size) {
    const crackitos_core::commons::fp max_size = 300.0f;
    const crackitos_core::commons::fp min_size = 16.0f;
    if (size > max_size) {
      return;
    }
    if (size < min_size) {
      return;
    }

    std::array<Tile, GRID_WIDTH*GRID_HEIGHT> temp{};

    for (std::size_t x = 0; x < map_.size(); x++) {
      map_[x].SetSizeTo(size);

      temp[x] = map_[x];
    }

    //resize and replace each element
    const auto usable_size = size;
    for(int x = 0; x < GRID_WIDTH; x++)
    {
      const auto X = static_cast<float>(x);
      for(int y = 0; y < GRID_HEIGHT; y++)
      {
        const auto Y = static_cast<float>(y);
        const auto index = x * GRID_WIDTH + y;
        auto replaced_tile = temp[index];
        replaced_tile.Shape().setPosition({X*usable_size, Y*usable_size});

        map_[index] = replaced_tile;
      }
    }
  }

  std::vector<Tile> &Map() { return map_; }
};

}

#endif //MICROMACHINE_GAME_INCLUDE_TILEMAP_H_
