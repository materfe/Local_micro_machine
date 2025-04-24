//
// Created by Mat on 4/23/2025.
//

#ifndef MICROMACHINE_GAME_INCLUDE_GAME_BOUNDS_H_
#define MICROMACHINE_GAME_INCLUDE_GAME_BOUNDS_H_

#include <array>
#include "physics_world.h"

namespace micromachine::game_bounds {
static constexpr std::int8_t number_of_bounds = 4;

class Wall
{
 private:

};

class Bounds {
 private :
  std::array<crackitos_physics::physics::Body, number_of_bounds> bounds_{};

 public:
  Bounds() = default;
};
}

#endif //MICROMACHINE_GAME_INCLUDE_GAME_BOUNDS_H_
