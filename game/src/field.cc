#include <random>
#include <iostream>

#include "field.h"

namespace micromachine::field {
void Tilemap::GeneratePath() {
  sf::Vector2i pos(GRID_WIDTH / 2, GRID_HEIGHT / 2);
  std::random_device gen;
  std::uniform_int_distribution distribution(0, 3);

  FulfillMap();
  FulfillVisited();

  //create the path
  for (int i = 0; i < GENERATION_AMOUNT; i++) {
    int dir = distribution(gen);
    if (dir == 0) pos.x++;
    if (dir == 1) pos.x--;
    if (dir == 2) pos.y++;
    if (dir == 3) pos.y--;

    // Bordures
    if (pos.x < 0 || pos.x >= GRID_WIDTH || pos.y < 0 || pos.y >= GRID_HEIGHT)
      continue;

    if (isVisited(pos.x, pos.y))
      continue;

    markVisited(pos.x, pos.y);
  }
  SetTextures();
}

void Tilemap::FulfillMap() {
  map_.clear();
  for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int x = 0; x < GRID_WIDTH; x++) {
      const auto new_tile = Tile({x, y});
      map_.emplace_back(new_tile);
    }
  }

}

void Tilemap::FulfillVisited() {
  for(auto& element : visited_)
  {
    element = false;
  }
}

void Tilemap::SetTextures() {
  sf::Texture roadTexture, grassTexture;

  std::string_view road_path("data/Tiles/Asphalt road/road_asphalt01.png");
  std::string_view grass_path("data/Tiles/Grass/land_grass11.png");

  if (roadTexture.loadFromFile(road_path)) {
    std::cerr << "AHHHHHHH, texture not good (asphalt)\n";
  }
  if (grassTexture.loadFromFile(grass_path)) {
    std::cerr << "AHHHHHHH, texture not good (grass)\n";
  }

  for (auto &_ : map_) {
    int x = _.Position().x;
    int y = _.Position().y;

    //check
    if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) {
      std::cerr << "Tile position out of bounds: (" << x << ", " << y << ")" << std::endl;
      continue;
    }

    if (isVisited(x, y)) {
      _.Shape().setTexture(&roadTexture);
    } else {
      _.Shape().setTexture(&grassTexture);
    }

  }
}
}