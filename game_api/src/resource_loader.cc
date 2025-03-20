//
// Created by Mat on 3/12/2025.
//

#include <iostream>
#include "resource_loader.h"

namespace Mat::Resource_loader
{
void Loader::LoadAllTextures() {
  const std::int16_t max = amount_of_textures - 1;
    for(std::int16_t i = 1; i < max; i++) //amount if text - 1 because of purpose empty
    {
      if(!all_textures_[i].loadFromFile("data/textures/circle_" + std::to_string(i) + ".png"))
      {
        break;
      }
    }
}
sf::Texture Loader::SelectSkin(Skins skin) {
  return all_textures_[static_cast<int>(skin)];
}
}