#include <random>
#include <iostream>

#include "field.h"

namespace micromachine::field {

std::random_device r;
std::default_random_engine e1(r());
std::uniform_int_distribution uniform_dist(1, 4);

void Tilemap::InitializeMap() {
  map_.clear();

  for (int x = 0; x < GRID_WIDTH; x++) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
      const auto new_tile = Tile({x * TILE_SIZE, y * TILE_SIZE});
      map_.emplace_back(new_tile);
    }
  }

}

void Tilemap::GenerateRandomMap() {
  InitializeMap();

  auto current_pos = sf::Vector2i(GRID_WIDTH / 2, GRID_HEIGHT / 2);

  for (int gen = 0; gen < GENERATION_NUMBER; gen++) {
    //random setup
    const int rnd = uniform_dist(e1);

    if (current_pos.x < 0 || current_pos.y < 0 || current_pos.x > WINDOW_WIDTH || current_pos.y > WINDOW_HEIGHT) {
      continue;
    }

    const auto index = current_pos.y * GRID_WIDTH + current_pos.x;

    if (map_[index].type() == TileType::Road) {
      current_pos = RandomPathUpdate(current_pos, rnd);
      continue;
    }

    map_[index].SetType(TileType::Road);

    current_pos = RandomPathUpdate(current_pos, rnd);
  }
  SetAllTextures();
}

sf::Vector2<int> &Tilemap::RandomPathUpdate(sf::Vector2<int> &current_pos, const int rnd) {
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

  std::string_view road_path("data/Tiles/Asphalt road/road_asphalt01.png");
  std::string_view grass_path("data/Tiles/Grass/land_grass11.png");

  if (!road_tex.loadFromFile(road_path)) {
    std::cerr << "PROBLEMS FOR ROAD TEXT\n";
  }
  if (!grass_tex.loadFromFile(grass_path)) {
    std::cerr << "PROBLEMS FOR GRASS TEXT\n";
  }

  for (auto &_ : map_) {
    if (_.type() == TileType::Road) {
      _.SetTextureTo(road_tex);
    } else {
      _.SetTextureTo(grass_tex);
    }
  }
}

}