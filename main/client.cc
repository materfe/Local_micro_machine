#include <SFML/Graphics/RenderWindow.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <iostream>

#include "const.h"
#include "playlist.h"
#include "car_game_manager.h"
#include "rendering_system.h"
#include "tilemap.h"
#include "camera.h"

namespace client {
crackitos_core::math::Vec2f UpdateDirection() {
  crackitos_core::math::Vec2f direction(0, 0);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) direction.y = -1.f;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) direction.y = 1.f;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) direction.x = -1.f;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) direction.x = 1.f;
  return direction;
}
crackitos_core::math::Vec2f UpdateDirection2() {
  crackitos_core::math::Vec2f direction(0, 0);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) direction.y = -1.f;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) direction.y = 1.f;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) direction.x = -1.f;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) direction.x = 1.f;
  return direction;
}
}

int main() {
  static constexpr std::int8_t player_amount = 3;
  static constexpr sf::Vector2f player_one_position(400.0f, 350.0f);
  static constexpr sf::Vector2f player_two_position(800.0f, 350.0f);
  static constexpr float player_radius = 25.0f;
  static constexpr sf::Color player_one_color(200, 35, 50, 255);
  static constexpr sf::Color player_two_color(30, 30, 255, 255);
  static constexpr sf::Color player_outline_color(233, 233, 0, 255);
  static constexpr float player_given_velocity = 10.0f;
  static constexpr float player_speed = 5.0f;

  micromachine::tilemap::Tilemap tilemap;
  micromachine::tilemap::Tilemap map;
  tilemap.GenerateRandomMap();
  map = tilemap;
  map.SetAllTileSizeTo(150.0f);
  micromachine::GameState game_state{};
  micromachine::View::Camera cam{};

  //1.
  //manager ----------------------------------------------------------------------------------
  micromachine::car_game_manager::Manager manager(player_amount);

  micromachine::player::Car player_three(game_state, {500.0f, 300.0f}, 1.0f, 1.0f, 100.0f);
  micromachine::player::Car player_four(game_state, {400.0f, 300.0f}, 1.0f, 1.0f, 100.0f);
  player_four.SetColor(sf::Color::White);
  micromachine::player::Car player_fifth(game_state, {300.0f, 300.0f}, 1.0f, 1.0f);
  player_fifth.SetColor(sf::Color::Red);
  manager.AddPlayer(player_three);
  manager.AddPlayer(player_four);
  manager.AddPlayer(player_fifth);
  manager.SetAllPositions(map.StartingPosition());

  //3.
  //set renderer ------------------------------------------------------------------------------
  auto render = micromachine::rendering::Renderer(WINDOW_WIDTH, WINDOW_HEIGHT, "MicroMachine");
  render.FrameRateLimit(60);
  render.VerticalSyncEnable(true);
  render.Window().setView(cam.view());
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

    while (const auto event = render.Window().pollEvent()) {
      ImGui::SFML::ProcessEvent(render.Window(), *event);
      if (event->is<sf::Event::Closed>()) {
        isOpen = false;
      }
    }

    const auto direction = client::UpdateDirection();
    const auto direction_2 = client::UpdateDirection2();


    //IMGUI --------------------------------------------------------------------------------------------------------
    ImGui::SFML::Update(render.Window(), deltaClock.restart());
    auto [x, y] = sf::Vector2<int>(WINDOW_WIDTH, WINDOW_HEIGHT);
    ImGui::SetNextWindowSize({(float) x, (float) y}, ImGuiCond_Always);
    ImGui::SetNextWindowPos({0.0f, 0.0f}, ImGuiCond_Always);
    ImGui::Begin("Simple Chat", nullptr, ImGuiWindowFlags_NoTitleBar);
    ImGui::End();
    ImGui::SFML::Render(render.Window());

    //Ticks --------------------------------------------------------------------------------------------------------
    game_state.Update(delta);


    cam.Update(manager);

    render.Window().setView(cam.view());

    manager.TicksAll(delta);

    player_three.Move(direction);
    player_four.Move(direction_2);

    //manage dying cars
    for (auto &car : manager.AllPlayers()) {
      if (!cam.IsInPlayableBounds(car)) {
        manager.KillPlayer(car);
      }
    }

    //DRAW ---------------------------------------------------------------------------------------------------------
    render.Clear();
    for (auto &tile : map.Map()) {
      render.Draw(tile.Shape());
    }
    for (auto &tile : tilemap.Map()) {
      render.Draw(tile.Shape());
    }
    for (auto &car : manager.AllPlayers()) {
      render.Draw(car.Shape());
    }
    for (auto &car : manager.AllDeadPlayers()) {
      render.Draw(car.Shape());
    }
    render.Display();
  }

  ImGui::SFML::Shutdown();
  return 0;
}