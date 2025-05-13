#include <random>
#include <iostream>
#include <set>

#include "tilemap.h"

namespace micromachine::tilemap {

std::random_device r;
std::default_random_engine e1(r());
std::uniform_int_distribution uniform_dist(1, 4);

void Tilemap::InitializeMap() {
  map_.clear();
  const auto size = 16;

  for (int x = 0; x < kGridWidth; x++) {
    for (int y = 0; y < kGridHeight; y++) {
      const auto new_tile = Tile({x * size, y * size});
      map_.emplace_back(new_tile);
    }
  }

}

void Tilemap::GenerateRandomMap() {
  InitializeMap();

  const int radius = std::min(kGridWidth, kGridHeight) / 3;
  const sf::Vector2i center(kGridWidth / 2, kGridHeight / 2);

  const int steps = 720;// plus de subdivisions = cercle plus lisse
  std::set<std::pair<int, int>> unique_positions;

  for (int i = 0; i < steps; ++i) {
    float angle = static_cast<float>(i) * (2 * crackitos_core::commons::kPi / steps);
    float fx = static_cast<float>(center.x) + radius * std::cos(angle);
    float fy = static_cast<float>(center.y) + radius * std::sin(angle);

    int x = static_cast<int>(std::round(fx));
    int y = static_cast<int>(std::round(fy));

    if (x == center.x && y == center.y) continue; // ignorer le centre

    // éviter les doublons
    if (unique_positions.insert({x, y}).second) {
      if (x >= 0 && x < kGridWidth && y >= 0 && y < kGridHeight) {
        int index = y * kGridWidth + x;
        map_[index].SetType(TileType::Road);
        road_.push_back(map_[index]);
      }
    }
  }

  ApplyTileRules();

  SetAllTextures();
}

void Tilemap::GenerateCheckpoints() {
  checkpoints_.clear();

  const int num_checkpoints = 8;
  const float angle_step = 2 * crackitos_core::commons::kPi / num_checkpoints;

  const int factor = 130;
  const auto radius = std::min(kGridWidth * factor, kGridHeight * factor) / 3;
  const sf::Vector2f center(
      static_cast<float>(kGridWidth * 157.0f / 2),
      static_cast<float>(kGridHeight * 157.0f / 2)
  );

  for (int i = 0; i < num_checkpoints; ++i) {
    float angle = static_cast<float>(i) * angle_step;

    float angle_rad = static_cast<float>(i) * angle_step;
    float angle_deg = angle_rad * 180.0f / static_cast<float>(crackitos_core::commons::kPi);

    float x = center.x + radius * std::cos(angle);
    float y = center.y + radius * std::sin(angle);

    checkpoints_.emplace_back(sf::Vector2f(x, y), angle_deg);
  }
}

bool Tilemap::IsRoad(const int x, const int y) {
  if (x < 0 || x >= kGridWidth || y < 0 || y >= kGridHeight)
    return false;
  int index = y * kGridWidth + x;
  return map_[index].type() == TileType::Road
      || map_[index].type() == TileType::StartRoad
      || map_[index].type() == TileType::EndRoad
      || map_[index].type() == TileType::HorizontalRoad
      || map_[index].type() == TileType::VerticalRoad
      || map_[index].type() == TileType::LeftUp
      || map_[index].type() == TileType::DownRight
      || map_[index].type() == TileType::DownLeft
      || map_[index].type() == TileType::UpRight
      || map_[index].type() == TileType::UpLeft;
}

void Tilemap::ApplyTileRules() {
  for (int y = 0; y < kGridHeight; ++y) {
    for (int x = 0; x < kGridWidth; ++x) {
      int index = y * kGridWidth + x;
      if (map_[index].type() != TileType::Road) continue;

      bool top = IsRoad(x, y - 1);
      bool bottom = IsRoad(x, y + 1);
      bool left = IsRoad(x - 1, y);
      bool right = IsRoad(x + 1, y);

      // Exemple simple : ligne horizontale
      if (left && right && !top && !bottom) {
        map_[index].SetType(TileType::HorizontalRoad);
      } else if (top && bottom && !left && !right) {
        map_[index].SetType(TileType::VerticalRoad);
      } else if (left and top and !right and !bottom) {
        map_[index].SetType(TileType::LeftUp);
      } else if (bottom and right and !top and !left) {
        map_[index].SetType(TileType::DownRight);
      } else if (bottom and left and !right and !top) {
        map_[index].SetType(TileType::DownLeft);
      } else if (top and right and !left and !bottom) {
        map_[index].SetType(TileType::UpRight);
      }
    }
  }
}

void Tilemap::SetAllTextures() {
  sf::Texture grass_tex;
  sf::Texture road_start;
  sf::Texture road_end;

  sf::Texture horizontal_road;
  sf::Texture vertical_road;
  sf::Texture left_up_corner;
  sf::Texture down_right_corner;
  sf::Texture down_left_corner;
  sf::Texture up_right_corner;

  std::string_view vertical_road_path("data/Tiles/Asphalt road/road_asphalt01.png");
  std::string_view horizontal_road_path("data/Tiles/Asphalt road/road_asphalt02.png");
  std::string_view left_up_path("data/Tiles/Asphalt road/road_asphalt25.png");
  std::string_view down_right_path("data/Tiles/Asphalt road/road_asphalt03.png");
  std::string_view down_left_path("data/Tiles/Asphalt road/road_asphalt05.png");
  std::string_view up_right_path("data/Tiles/Asphalt road/road_asphalt24.png");

  std::string_view grass_path("data/Tiles/Grass/land_grass11.png");
  std::string_view road_start_path("data/Tiles/Asphalt road/start_road.png");
  std::string_view road_end_path("data/Tiles/Asphalt road/end_road.png");

  if (!vertical_road.loadFromFile(vertical_road_path)) {
    std::cerr << "PROBLEMS FOR ROAD TEXT\n";
  }
  if (!horizontal_road.loadFromFile(horizontal_road_path)) {
    std::cerr << "PROBLEMS FOR ROAD TEXT\n";
  }
  if (!left_up_corner.loadFromFile(left_up_path)) {
    std::cerr << "PROBLEMS FOR ROAD TEXT\n";
  }
  if (!down_right_corner.loadFromFile(down_right_path)) {
    std::cerr << "PROBLEMS FOR ROAD TEXT\n";
  }
  if (!up_right_corner.loadFromFile(up_right_path)) {
    std::cerr << "PROBLEMS FOR ROAD TEXT\n";
  }
  if (!down_left_corner.loadFromFile(down_left_path)) {
    std::cerr << "PROBLEMS FOR ROAD TEXT\n";
  }

  if (!grass_tex.loadFromFile(grass_path)) {
    std::cerr << "PROBLEMS FOR GRASS TEXT\n";
  }
  if (!road_end.loadFromFile(road_end_path)) {
    std::cerr << "PROBLEMS FOR END ROAD TEXT\n";
  }
  if (!road_start.loadFromFile(road_start_path)) {
    std::cerr << "PROBLEMS FOR START ROAD TEXT\n";
  }

  for (auto &_ : map_) {
    //horizontal + vertical
    if (_.type() == TileType::VerticalRoad) {
      _.SetTextureTo(horizontal_road);
    } else if (_.type() == TileType::HorizontalRoad) {
      _.SetTextureTo(vertical_road);
    } else if (_.type() == TileType::LeftUp) {
      _.SetTextureTo(left_up_corner);
    } else if (_.type() == TileType::DownRight) {
      _.SetTextureTo(down_right_corner);
    } else if (_.type() == TileType::DownLeft) {
      //bugged
      _.SetTextureTo(up_right_corner);
    } else if (_.type() == TileType::UpRight) {
      //bugged
      _.SetTextureTo(down_left_corner);
    } else if (_.type() == TileType::Grass) {
      _.SetTextureTo(grass_tex);
    } else if (_.type() == TileType::StartRoad) {
      _.SetTextureTo(road_start);
    } else {
      _.SetTextureTo(road_end);
    }
  }
}

void Tilemap::SetAllTileSizeTo(crackitos_core::commons::fp size) {
  const crackitos_core::commons::fp max_size = 300.0f;
  const crackitos_core::commons::fp min_size = 16.0f;
  if (size > max_size) {
    return;
  }
  if (size < min_size) {
    return;
  }

  std::array<Tile, kGridWidth * kGridHeight> temp{};

  for (std::size_t x = 0; x < map_.size(); x++) {
    map_[x].SetSizeTo(size);

    temp[x] = map_[x];
  }

  //resize and replace each element
  const auto usable_size = size;
  for (int x = 0; x < kGridWidth; x++) {
    const auto X = static_cast<float>(x);
    for (int y = 0; y < kGridHeight; y++) {
      const auto Y = static_cast<float>(y);
      const auto index = x * kGridWidth + y;
      auto replaced_tile = temp[index];
      replaced_tile.Shape().setPosition({X * usable_size, Y * usable_size});

      map_[index] = replaced_tile;
    }
  }
}


}