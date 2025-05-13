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
#include "vec2.h"
#include "check.h"

namespace micromachine::tilemap {
static constexpr std::int8_t kGridWidth = 20;
static constexpr std::int8_t kGridHeight = 20;

enum class TileType {
  Grass,
  Road,
  StartRoad,
  EndRoad,
  HorizontalRoad,
  VerticalRoad,
  UpLeft,
  UpRight,
  DownLeft,
  DownRight,
  LeftUp
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
    //shape_.setOutlineThickness(-1.0f);
    //shape_.setOutlineColor(sf::Color::Red);
    SetSizeTo(16.0f);
  }

  Tile() = default;

  void SetType(TileType type) { type_ = type; }
  TileType type() { return type_; }
  crackitos_core::commons::fp &Size() { return size_; }
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
  std::vector<Checkpoint> checkpoints_{};

  void InitializeMap();
  void SetAllTextures();
  [[nodiscard]] bool IsRoad(int x, int y);
  void ApplyTileRules();

 public:
  Tilemap() {
    map_.reserve(kGridWidth * kGridHeight);
    road_.reserve(kGridWidth * kGridHeight);
  }

  void GenerateRandomMap();
  void GenerateCheckpoints();
  void SetAllTileSizeTo(crackitos_core::commons::fp size);

  std::vector<Tile> &Map() { return map_; }
  std::vector<Checkpoint> &checkpoints() { return checkpoints_; }
};

}

#endif //MICROMACHINE_GAME_INCLUDE_TILEMAP_H_
