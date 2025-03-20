//
// Created by Mat on 3/12/2025.
//

#ifndef POWEROF4_GAME_API_INCLUDE_RESOURCE_LOADER_H_
#define POWEROF4_GAME_API_INCLUDE_RESOURCE_LOADER_H_

#include <array>
#include <SFML/Graphics.hpp>

namespace Mat::Resource_loader {

enum class Skins {
  PurposeEmpty,
  First,
  Second,
  Third,
  Fourth,
  Fifth,
  Sixth,
  Seventh,
  Eighth,
  Ninth
};

static constexpr std::int16_t amount_of_textures = 10;

class Loader{
 public:
 Loader() {LoadAllTextures();}

 sf::Texture SelectSkin(Skins skin);


 private:
  std::array<sf::Texture, amount_of_textures> all_textures_{};

 void LoadAllTextures();
};
}

#endif //POWEROF4_GAME_API_INCLUDE_RESOURCE_LOADER_H_
