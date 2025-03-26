#include <SFML/Graphics/RenderWindow.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <SFML/Graphics.hpp>

#include <iostream>

#include "playlist.h"
#include "car_game_manager.h"

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

  //manager
  micromachine::car_game_manager::Manager manager(player_amount);
  //player one
  micromachine::player::Car player_one(player_one_position);
  micromachine::player::Car player_two(player_two_position);
  manager.AddPlayer(player_one);
  manager.AddPlayer(player_two);

  auto player_representation_01 = sf::CircleShape(player_radius, 3); //making triangles
  player_representation_01.setFillColor(player_one_color);
  player_representation_01.setOutlineColor(player_outline_color);
  player_representation_01.setOutlineThickness(-2.0f);

  auto player_representation_02 = sf::CircleShape(player_radius, 3); //making triangles
  player_representation_02.setFillColor(player_two_color);
  player_representation_02.setOutlineColor(player_outline_color);
  player_representation_02.setOutlineThickness(-2.0f);

  sf::RenderWindow window(sf::VideoMode({1280, 720}), "connect_8");
  window.setFramerateLimit(60);
  if (!ImGui::SFML::Init(window)) {
    std::cerr << "imgui error";
    return EXIT_FAILURE;
  }

  bool isOpen = true;
  sf::Clock deltaClock;

  std::int8_t framerate_limit = 60;
  window.setFramerateLimit(framerate_limit);
  window.setVerticalSyncEnabled(true);

  while (isOpen) {

    auto delta = deltaClock.getElapsedTime().asMilliseconds();

    while (const auto event = window.pollEvent()) {
      ImGui::SFML::ProcessEvent(window, *event);
      if (event->is<sf::Event::Closed>()) {
        isOpen = false;
      }
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
        }
      }
    }


    //IMGUI --------------------------------------------------------------------------------------------------------
    ImGui::SFML::Update(window, deltaClock.restart());
    auto [x, y] = window.getSize();
    ImGui::SetNextWindowSize({(float) x, (float) y}, ImGuiCond_Always);
    ImGui::SetNextWindowPos({0.0f, 0.0f}, ImGuiCond_Always);
    ImGui::Begin("Simple Chat", nullptr, ImGuiWindowFlags_NoTitleBar);
    ImGui::End();
    ImGui::SFML::Render(window);

    //Ticks --------------------------------------------------------------------------------------------------------
    manager.AllTicks(static_cast<float>(delta));

    player_representation_01.setPosition(manager.AllPlayers()[0].Position());
    player_representation_02.setPosition(manager.AllPlayers()[1].Position());


    //DRAW ---------------------------------------------------------------------------------------------------------
    window.clear();
    window.draw(player_representation_01);
    window.draw(player_representation_02);
    window.display();
  }

  ImGui::SFML::Shutdown();
  return 0;
}