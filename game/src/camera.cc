//
// Created by Mat on 5/13/2025.
//

#include "camera.h"

namespace micromachine::View
{
void Camera::UpdateBounds() {
  const auto half_width = view_.getSize().x / 2;
  const auto half_height = view_.getSize().y / 2;

  crackitos_core::math::Vec2f min_bounds(view_.getCenter().x - half_width,
                                         view_.getCenter().y - half_height);
  crackitos_core::math::Vec2f max_bounds(view_.getCenter().x + half_width,
                                         view_.getCenter().y + half_height);

  bounds_.set_min_bound(min_bounds);
  bounds_.set_max_bound(max_bounds);
}
}