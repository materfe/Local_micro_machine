//
// Created by Mat on 4/10/2025.
//

#ifndef MICROMACHINE_GAME_INCLUDE_GAME_STATE_H_
#define MICROMACHINE_GAME_INCLUDE_GAME_STATE_H_

#include "physics_world.h"
#include "const.h"

namespace micromachine {

class GameState {
  crackitos_physics::physics::PhysicsWorld world_{};

 public:
  GameState(){
    const auto min_bounds = crackitos_core::math::Vec2f(WINDOW_WIDTH * -20.0f, WINDOW_HEIGHT * -20.0f);
    const auto max_bounds = crackitos_core::math::Vec2f(WINDOW_WIDTH * 20.0f, WINDOW_HEIGHT * 20.0f);
    const auto screen_bounds = crackitos_core::math::AABB(min_bounds, max_bounds);
    const auto out_of_bounce_removal = true;
    const auto gravity_value = crackitos_core::math::Vec2f(0.0f, 0.0f);

    world_.Initialize(screen_bounds, out_of_bounce_removal, gravity_value);
  }

  crackitos_physics::physics::BodyHandle CreateBody(const crackitos_physics::physics::BodyType &type,
                                                    const crackitos_core::math::Vec2f &pos,
                                                    const crackitos_core::commons::fp mass) {

    const auto velocity = crackitos_core::math::Vec2f::Zero();
    const auto body_def = crackitos_physics::physics::Body(type, pos, velocity, false, mass);

    return world_.CreateBody(body_def);
  }

  crackitos_physics::physics::ColliderHandle CreateCollider(const crackitos_physics::physics::BodyHandle &body,
                                                            const crackitos_core::math::AABB &bounds,
                                                            const crackitos_core::commons::fp bounciness,
                                                            const bool is_trigger) {

    const crackitos_core::commons::fp friction_value = 0.0f;
    const auto collider = crackitos_physics::physics::Collider(bounds, bounciness, friction_value, is_trigger, body);

    return world_.CreateCollider(body, collider);
  }

  void Update(const crackitos_core::commons::fp delta_time)
  {
    world_.Update(delta_time);
  }

  crackitos_physics::physics::PhysicsWorld& World(){return world_;}
};
}

#endif //MICROMACHINE_GAME_INCLUDE_GAME_STATE_H_
