#include <SFML/Graphics/RenderWindow.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <iostream>

#include "const.h"
#include "car_game_manager.h"
#include "rendering_system.h"
#include "tilemap.h"
#include "camera.h"
#include "online_client.h"

namespace client {
void UpdateDirection() {
  crackitos_core::math::Vec2f direction(0, 0);
  if (micromachine::NetworkManager::GetLoadBalancingClient().getLocalPlayer().getNumber() == 2) {
    //std::cout << "nope, refused\n";
    return;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) direction.y = -1.f;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) direction.y = 1.f;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) direction.x = -1.f;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) direction.x = 1.f;

  //send to clients
  char buf[64];
  std::snprintf(buf, sizeof(buf), "%f,%f", direction.x, direction.y);
  ExitGames::Common::JString jsDir(buf);

  micromachine::NetworkManager::GetLoadBalancingClient().opRaiseEvent(true, jsDir, 2);
}
}

int main() {
  static constexpr std::int8_t player_amount = 2;
  static constexpr sf::Vector2f player_one_position(1500.0f, 700.0f);
  static constexpr sf::Vector2f player_two_position(800.0f, 350.0f);
  static constexpr float player_radius = 25.0f;
  static constexpr sf::Color player_one_color(200, 35, 50, 255);
  static constexpr sf::Color player_two_color(30, 30, 255, 255);
  static constexpr sf::Color player_outline_color(233, 233, 0, 255);
  static constexpr float player_given_velocity = 10.0f;
  static constexpr float player_speed = 5.0f;

  //set client
  micromachine::MyClient client;
  ExitGames::LoadBalancing::ClientConstructOptions options;
  micromachine::NetworkManager::Begin(&client, options);

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
  micromachine::player::Car player_four(game_state, {500.0f, 200.0f}, 1.0f, 1.0f, 100.0f);
  player_four.SetColor(sf::Color::White);
  manager.AddPlayer(player_three);
  manager.AddPlayer(player_four);
  manager.SetAllPositions({player_one_position.x, player_one_position.y});

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

  crackitos_core::timer::Timer timer;
  timer.SetFixedDeltaTime(1.0f / 60.0f);

  cam.view().zoom(3.0f);
  cam.view().move({1200, 1100});

  //4.
  //main loop ---------------------------------------------------------------------------------
  bool isOpen = true;
  sf::Clock deltaClock;
  while (isOpen) {

    const int remoteNr = client.getRemotePlayerNr();
    const int localNr = client.getLocalPlayerNr();

    micromachine::NetworkManager::Tick();

    const auto delta = deltaClock.getElapsedTime().asSeconds();

    while (const auto event = render.Window().pollEvent()) {
      ImGui::SFML::ProcessEvent(render.Window(), *event);
      if (event->is<sf::Event::Closed>()) {
        isOpen = false;
      }
    }


    //IMGUI --------------------------------------------------------------------------------------------------------
    ImGui::SFML::Update(render.Window(), deltaClock.restart());
    auto [x, y] = sf::Vector2<int>(WINDOW_WIDTH, WINDOW_HEIGHT);
    ImGui::SetNextWindowSize({(float) x, (float) y}, ImGuiCond_Always);
    ImGui::SetNextWindowPos({0.0f, 0.0f}, ImGuiCond_Always);
    ImGui::Begin("Micromachine", nullptr, ImGuiWindowFlags_NoTitleBar);
    ImGui::End();
    ImGui::SFML::Render(render.Window());

    //Ticks --------------------------------------------------------------------------------------------------------

    //update physique
    timer.Tick();

    // Gestion des inputs
    float directionX = 0;
    float directionY = 0;
    if (render.Window().hasFocus()) {
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) directionY = -1.f;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) directionY = 1.f;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) directionX = -1.f;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) directionX = 1.f;
    }

    //--------------FIXED UPDATE----------------
    while (timer.FixedDeltaTimeStep()) {
      game_state.Update(delta);
    }

    //cam.Update(manager);



    render.Window().setView(cam.view());

    manager.TicksAll(delta);

    if (localNr == 1) {
      player_three.Move({directionX, directionY});         // toi = player
      player_four.Move({client.direction2().x, client.direction2().y});             // autre = player2
    }
    else {
      player_three.Move({client.direction1().x, client.direction1().y});              // autre = player
      player_four.Move({directionX, directionY});        // toi = player2
    }

    char buf[64];
    std::snprintf(buf, sizeof(buf), "%f,%f", directionX, directionY);
    ExitGames::Common::JString jsDir(buf);

    micromachine::NetworkManager::GetLoadBalancingClient().opRaiseEvent(false, jsDir, 2);

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

  micromachine::NetworkManager::End();
  ImGui::SFML::Shutdown();
  return 0;
}