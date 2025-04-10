//
// Created by Mat on 4/2/2025.
//

#ifndef MICROMACHINE_GAME_INCLUDE_RENDERING_SYSTEM_H_
#define MICROMACHINE_GAME_INCLUDE_RENDERING_SYSTEM_H_
#include <SFML/Graphics.hpp>

namespace micromachine::rendering
{

class Renderer
{
 private:
  //TODO rendering.cc
  sf::RenderWindow window_;

 public:
  Renderer(uint32_t width, uint32_t height, std::string_view name)
  {
    auto window_size = sf::VideoMode({width, height});
    window_.create(window_size, name.data());
  }

  void Draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default)
  {
    window_.draw(drawable);
  }

  void Clear(const sf::Color color = sf::Color::Black)
  {
    window_.clear(color);
  }

  void Display()
  {
    window_.display();
  }

  void FrameRateLimit(uint32_t limit)
  {
    window_.setFramerateLimit(limit);
  }

  void VerticalSyncEnable(bool value)
  {
    window_.setVerticalSyncEnabled(value);
  }

  sf::RenderWindow& Window(){return window_;}
};

}



#endif //MICROMACHINE_GAME_INCLUDE_RENDERING_SYSTEM_H_
