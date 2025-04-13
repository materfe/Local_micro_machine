#include <SFML/Graphics/RenderWindow.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <SFML/Graphics.hpp>

#include <iostream>

#include "const.h"
#include "playlist.h"
#include "car_game_manager.h"
#include "rendering_system.h"
#include "tilemap.h"


namespace client {
void Events(micromachine::car_game_manager::Manager &manager,
            const std::optional<sf::Event> &event) {
  if (event->is<sf::Event::KeyPressed>()) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
      manager.AllPlayers()[0].Options().TurnAccelerationTo(true);
      manager.AllPlayers()[0].Options().TurnBrakingTo(false);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
      manager.AllPlayers()[0].Options().TurnTurningLeftTo(true);
      manager.AllPlayers()[0].Options().TurnTurningRightTo(false);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
      manager.AllPlayers()[0].Options().TurnTurningRightTo(true);
      manager.AllPlayers()[0].Options().TurnTurningLeftTo(false);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
      manager.AllPlayers()[0].Options().TurnBrakingTo(true);
      manager.AllPlayers()[0].Options().TurnAccelerationTo(false);
    }
  }
}

crackitos_core::math::Vec2f UpdateDirection() {
  crackitos_core::math::Vec2f direction(0, 0);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) direction.y = -1.f;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) direction.y = 1.f;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) direction.x = -1.f;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) direction.x = 1.f;
  return direction;
}
}

int main() {
  static constexpr std::int8_t player_amount = 10;
  static constexpr sf::Vector2f player_one_position(400.0f, 350.0f);
  static constexpr sf::Vector2f player_two_position(800.0f, 350.0f);
  static constexpr float player_radius = 25.0f;
  static constexpr sf::Color player_one_color(200, 35, 50, 255);
  static constexpr sf::Color player_two_color(30, 30, 255, 255);
  static constexpr sf::Color player_outline_color(233, 233, 0, 255);
  static constexpr float player_given_velocity = 10.0f;
  static constexpr float player_speed = 5.0f;

  micromachine::field::Tilemap tilemap;
  tilemap.GenerateRandomMap();

  micromachine::GameState game_state{};

  //1.
  //manager ----------------------------------------------------------------------------------
  micromachine::car_game_manager::Manager manager(player_amount);
  //player one
  micromachine::player::Car player_one(player_one_position);
  micromachine::player::Car player_two(player_two_position);

  micromachine::player::Cars player_three(game_state, {500.0f,300.0f}, 1.0f, 0.5f);
  manager.AddPlayer(player_one);
  manager.AddPlayer(player_two);

  //2.
  //set shapes --------------------------------------------------------------------------------
  auto player_representation_01 = sf::CircleShape(player_radius, 3);
  player_representation_01.setFillColor(player_one_color);
  player_representation_01.setOutlineColor(player_outline_color);
  player_representation_01.setOutlineThickness(-2.0f);

  auto player_representation_02 = sf::CircleShape(player_radius, 3);
  player_representation_02.setFillColor(player_two_color);
  player_representation_02.setOutlineColor(player_outline_color);
  player_representation_02.setOutlineThickness(-2.0f);

  //3.
  //set renderer ------------------------------------------------------------------------------
  auto render = micromachine::rendering::Renderer(WINDOW_WIDTH, WINDOW_HEIGHT, "MicroMachine");
  render.FrameRateLimit(60);
  render.VerticalSyncEnable(true);
  if (!ImGui::SFML::Init(render.Window())) {
    std::cerr << "imgui error";
    return EXIT_FAILURE;
  }

  //4.
  //main loop ---------------------------------------------------------------------------------
  bool isOpen = true;
  sf::Clock deltaClock;
  while (isOpen) {

    const auto delta = deltaClock.getElapsedTime().asSeconds();

    game_state.Update(delta);

    while (const auto event = render.Window().pollEvent()) {
      ImGui::SFML::ProcessEvent(render.Window(), *event);
      if (event->is<sf::Event::Closed>()) {
        isOpen = false;
      }
      client::Events(manager, event);
    }

    const auto direction = client::UpdateDirection();


    //IMGUI --------------------------------------------------------------------------------------------------------
    ImGui::SFML::Update(render.Window(), deltaClock.restart());
    auto [x, y] = sf::Vector2<int>(WINDOW_WIDTH, WINDOW_HEIGHT);
    ImGui::SetNextWindowSize({(float) x, (float) y}, ImGuiCond_Always);
    ImGui::SetNextWindowPos({0.0f, 0.0f}, ImGuiCond_Always);
    ImGui::Begin("Simple Chat", nullptr, ImGuiWindowFlags_NoTitleBar);
    ImGui::End();
    ImGui::SFML::Render(render.Window());

    //Ticks --------------------------------------------------------------------------------------------------------
    manager.TicksAll(static_cast<float>(delta));

    player_representation_01.setPosition(manager.AllPlayers()[0].Position());
    //player_representation_01.rotate(sf::radians(manager.AllPlayers()[0].Options().angle));
    player_representation_02.setPosition(manager.AllPlayers()[1].Position());
    game_state.Update(delta);

    player_three.Move(direction);
    player_three.Update(delta);

    //DRAW ---------------------------------------------------------------------------------------------------------
    render.Clear();
    render.Draw(player_representation_01);
    render.Draw(player_representation_02);
    render.Draw(player_three.Shape());
    for (auto &tile : tilemap.Map()) {
      render.Draw(tile.Shape());
    }
    render.Display();
  }

  ImGui::SFML::Shutdown();
  return 0;
}

