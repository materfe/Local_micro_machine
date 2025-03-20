//
// Created by Mat on 3/12/2025.
//

#ifndef POWEROF4_GAME_API_INCLUDE_CLIENT_GAME_TUNING_H_
#define POWEROF4_GAME_API_INCLUDE_CLIENT_GAME_TUNING_H_

#include "playground.h"
#include "resource_loader.h"

namespace Mat::Client::Tuning {

class Client {
  Mat::Resource_loader::Loader loader_{};
 public:
  sf::Texture current_skin_;
  const int radius = 20;
  const int spacing = 40;
  power_of_four::Playground playground_{};
  Client() = default;

  void SelectSkin(Mat::Resource_loader::Skins skin) { current_skin_ = loader_.SelectSkin(skin); }
};

}

#endif //POWEROF4_GAME_API_INCLUDE_CLIENT_GAME_TUNING_H_
