#include <random>
#include <iostream>

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

  auto current_pos = sf::Vector2i(GRID_WIDTH / 2, GRID_HEIGHT / 2);

  auto gen_number = 0;

  while (gen_number < GENERATION_NUMBER) {
    //random setup
    const int rnd = uniform_dist(e1);

    if (current_pos.x < 0 || current_pos.y < 0 || current_pos.x > WINDOW_WIDTH || current_pos.y > WINDOW_HEIGHT) {
      continue;
    }

    const auto index = current_pos.y * GRID_WIDTH + current_pos.x;

    if (map_[index].type() == TileType::Road) {
      current_pos = ChooseNeighbour(current_pos, rnd);
      continue;
    }

    map_[index].SetType(TileType::Road);
    const auto tile = map_[index];
    road_.push_back(tile);
    gen_number++;
    current_pos = ChooseNeighbour(current_pos, rnd);
  }

  SetStartAndEnd();

  SetAllTextures();
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

sf::Vector2<int> &Tilemap::ChooseNeighbour(sf::Vector2<int> &current_pos, const int rnd) {
  switch (rnd) {
    case 1:current_pos.x++;
      break;
    case 2:current_pos.x--;
      break;
    case 3:current_pos.y++;
      break;
    case 4:current_pos.y--;
      break;
    default:break;
  }
  return current_pos;
}

void Tilemap::SetAllTextures() {
  sf::Texture road_tex;
  sf::Texture grass_tex;
  sf::Texture road_start;
  sf::Texture road_end;

  std::string_view road_path("data/Tiles/Asphalt road/road_asphalt01.png");
  std::string_view grass_path("data/Tiles/Grass/land_grass11.png");
  std::string_view road_start_path("data/Tiles/Asphalt road/start_road.png");
  std::string_view road_end_path("data/Tiles/Asphalt road/end_road.png");

  if (!road_tex.loadFromFile(road_path)) {
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
    if (_.type() == TileType::Road) {
      _.SetTextureTo(road_tex);
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