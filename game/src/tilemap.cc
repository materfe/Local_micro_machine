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

  for (int x = 0; x < GRID_WIDTH; x++) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
      const auto new_tile = Tile({x * size, y * size});
      map_.emplace_back(new_tile);
    }
  }

}

void Tilemap::GenerateRandomMap() {
  InitializeMap();

  const int radius = std::min(GRID_WIDTH, GRID_HEIGHT) / 3;
  const sf::Vector2i center(GRID_WIDTH / 2, GRID_HEIGHT / 2);

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
      if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT) {
        int index = y * GRID_WIDTH + x;
        map_[index].SetType(TileType::Road);
        road_.push_back(map_[index]);
      }
    }
  }

  ApplyTileRules();
  SetStartAndEnd();

  SetAllTextures();
}

bool Tilemap::IsRoad(const int x, const int y) {
  if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT)
    return false;
  int index = y * GRID_WIDTH + x;
  return map_[index].type() == TileType::Road
      || map_[index].type() == TileType::StartRoad
      || map_[index].type() == TileType::EndRoad
      || map_[index].type() == TileType::HorizontalRoad
      || map_[index].type() == TileType::VerticalRoad
      || map_[index].type() == TileType::LeftUp
      || map_[index].type() == TileType::DownRight
      || map_[index].type() == TileType::DownLeft;
}

void Tilemap::ApplyTileRules() {
  for (int y = 0; y < GRID_HEIGHT; ++y) {
    for (int x = 0; x < GRID_WIDTH; ++x) {
      int index = y * GRID_WIDTH + x;
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
      }
    }
  }
}

void Tilemap::SetStartAndEnd() {
  if (map_.empty()) return;

  float min_y = std::numeric_limits<float>::infinity();
  auto kept_index_start = 0;
  float max_y = std::numeric_limits<float>::lowest();
  auto kept_index_end = 0;

  for (int x = 0; x < GRID_WIDTH; x++) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
      const auto index = x * GRID_WIDTH + y;
      //fast return
      if (map_[index].type() != TileType::Road) {
        continue;
      }

      float tile_y = map_[index].Shape().getPosition().y;

      if (tile_y < min_y) {
        min_y = tile_y;
        kept_index_end = index;
      } else if (tile_y > max_y) {
        max_y = tile_y;
        kept_index_start = index;
      }
    }
  }

  map_[kept_index_end].SetType(TileType::EndRoad);
  map_[kept_index_start].SetType(TileType::StartRoad);
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

  std::string_view vertical_road_path("data/Tiles/Asphalt road/road_asphalt01.png");
  std::string_view horizontal_road_path("data/Tiles/Asphalt road/road_asphalt02.png");
  std::string_view left_up_path("data/Tiles/Asphalt road/road_asphalt25.png");
  std::string_view down_right_path("data/Tiles/Asphalt road/road_asphalt03.png");
  std::string_view down_left_path("data/Tiles/Asphalt road/road_asphalt05.png");

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
      _.SetTextureTo(down_left_corner);
    } else if (_.type() == TileType::Road) {
      _.SetTextureTo(horizontal_road);
    } else if (_.type() == TileType::Grass) {
      _.SetTextureTo(grass_tex);
    } else if (_.type() == TileType::StartRoad) {
      _.SetTextureTo(road_start);
    } else {
      _.SetTextureTo(road_end);
    }
  }
}

crackitos_core::math::Vec2f Tilemap::StartingPosition() {
  crackitos_core::math::Vec2f return_pos{};
  for (auto &tile : map_) {
    if (tile.type() != TileType::StartRoad) {
      continue;
    }

    return_pos = {tile.Shape().getPosition().x, tile.Shape().getPosition().y};
  }
  return return_pos;
}
}