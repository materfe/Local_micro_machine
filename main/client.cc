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
#include "field.h"

namespace client {
void Events(float player_given_velocity,
            float player_speed,
            micromachine::car_game_manager::Manager &manager,
            const std::optional<sf::Event> &event) {
  if (event->is<sf::Event::KeyPressed>()) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
      sf::Vector2<float> new_velocity_01 = sf::Vector2f(0.0f, -1 * (player_given_velocity * player_speed));
      sf::Vector2<float> new_velocity_02 = sf::Vector2f(0.0f, (player_given_velocity * player_speed));

      //player_one.MoveAt(new_velocity);
      manager.AllPlayers()[0].MoveAt(new_velocity_01);
      manager.AllPlayers()[1].MoveAt(new_velocity_02);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
      sf::Vector2<float> new_velocity_01 = sf::Vector2f(0.0f, player_given_velocity * player_speed);
      sf::Vector2<float> new_velocity_02 = sf::Vector2f(0.0f, -1 * (player_given_velocity * player_speed));

      //player_one.MoveAt(new_velocity);
      manager.AllPlayers()[0].MoveAt(new_velocity_01);
      manager.AllPlayers()[1].MoveAt(new_velocity_02);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
      sf::Vector2<float> new_velocity_01 = sf::Vector2f(-1 * player_given_velocity * player_speed, 0.0f);
      sf::Vector2<float> new_velocity_02 = sf::Vector2f(player_given_velocity * player_speed, 0.0f);

      //player_one.MoveAt(new_velocity);
      manager.AllPlayers()[0].MoveAt(new_velocity_01);
      manager.AllPlayers()[1].MoveAt(new_velocity_02);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
      sf::Vector2<float> new_velocity_01 = sf::Vector2f(player_given_velocity * player_speed, 0.0f);
      sf::Vector2<float> new_velocity_02 = sf::Vector2f(-1 * (player_given_velocity * player_speed), 0.0f);

      //player_one.MoveAt(new_velocity);
      manager.AllPlayers()[0].MoveAt(new_velocity_01);
      manager.AllPlayers()[1].MoveAt(new_velocity_02);
    } else {
      sf::Vector2<float> new_velocity_0 = sf::Vector2f (0.0f, 0.0f);

      //player_one.MoveAt(new_velocity);
      manager.AllPlayers()[0].MoveAt(new_velocity_0);
      manager.AllPlayers()[1].MoveAt(new_velocity_0);
    }
  }
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
  static constexpr float player_given_velocity = 30.0f;
  static constexpr float player_speed = 5.0f;

  micromachine::field::Tilemap tilemap;
  tilemap.GeneratePath();

  //1.
  //manager ----------------------------------------------------------------------------------
  micromachine::car_game_manager::Manager manager(player_amount);
  //player one
  micromachine::player::Car player_one(player_one_position);
  micromachine::player::Car player_two(player_two_position);
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

    auto delta = deltaClock.getElapsedTime().asMilliseconds();

    while (const auto event = render.Window().pollEvent()) {
      ImGui::SFML::ProcessEvent(render.Window(), *event);
      if (event->is<sf::Event::Closed>()) {
        isOpen = false;
      }
      client::Events(player_given_velocity, player_speed, manager, event);
    }


    //IMGUI --------------------------------------------------------------------------------------------------------
    ImGui::SFML::Update(render.Window(), deltaClock.restart());
    auto [x, y] = sf::Vector2<int>(WINDOW_WIDTH, WINDOW_HEIGHT);
    ImGui::SetNextWindowSize({(float) x, (float) y}, ImGuiCond_Always);
    ImGui::SetNextWindowPos({0.0f, 0.0f}, ImGuiCond_Always);
    ImGui::Begin("Simple Chat", nullptr, ImGuiWindowFlags_NoTitleBar);
    ImGui::End();
    ImGui::SFML::Render(render.Window());

    //Ticks --------------------------------------------------------------------------------------------------------
    manager.AllTicks(static_cast<float>(delta));

    player_representation_01.setPosition(manager.AllPlayers()[0].Position());
    player_representation_02.setPosition(manager.AllPlayers()[1].Position());

    //DRAW ---------------------------------------------------------------------------------------------------------
    render.Clear();
    render.Draw(player_representation_01);
    render.Draw(player_representation_02);
    for(auto& tile : tilemap.Map())
    {
      render.Draw(tile.Shape());
    }
    render.Display();
  }

  ImGui::SFML::Shutdown();
  return 0;
}
