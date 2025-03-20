#include <SFML/Graphics/RenderWindow.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>

#include "const.h"
#include "playlist.h"
#include "client_game_tuning.h"

namespace Mat::client {
enum class Status {
  NOT_CONNECTED,
  CONNECTED
};
}

int main() {
  Mat::Client::Tuning::Client client{};
  auto circle = sf::CircleShape(static_cast<float>(client.radius));
  auto position = sf::Vector2<float>(100, 100);
  circle.setFillColor(sf::Color::Green);
  circle.setPosition(position);
  client.SelectSkin(Mat::Resource_loader::Skins::Seventh); //select skin number seven
  circle.setTexture(&client.current_skin_);

  sf::RenderWindow window(sf::VideoMode({1280, 720}), "connect_8");
  window.setFramerateLimit(60);
  if(!ImGui::SFML::Init(window))
  {
    std::cerr << "imgui error";
    return EXIT_FAILURE;
  }

  bool isOpen = true;
  sf::Clock deltaClock;
  Mat::client::Status status = Mat::client::Status::NOT_CONNECTED;

  std::vector<std::string> receivedMessages;

  sf::TcpSocket socket;
  socket.setBlocking(false);
  std::string serverAddress = "localhost";
  serverAddress.resize(50, 0);
  short portNumber = PORT_NUMBER;
  //message you send
  std::string sendMessage;
  sendMessage.resize(MAX_MESSAGE_LENGTH, 0);

  //index send when a player tries to take a tile
  std::string message;
  message.resize(MAX_MESSAGE_LENGTH, 0);

  //message received
  std::string receivedMessage;
  receivedMessage.resize(MAX_MESSAGE_LENGTH, 0);

  //received instructions from serv
  std::string received_instruction_index;
  received_instruction_index.resize(MAX_MESSAGE_LENGTH, 0);
  std::string received_instruction_player;
  received_instruction_player.resize(MAX_MESSAGE_LENGTH, 0);

  while (isOpen) {
    while (const std::optional event = window.pollEvent()) {
      ImGui::SFML::ProcessEvent(window, *event);
      if (event->is<sf::Event::Closed>()) {
        isOpen = false;
      }
      if (event->is<sf::Event::MouseButtonPressed>()) {
        auto mousePos = sf::Mouse::getPosition(window);

        // Find which column was clicked
        int clickedCol = (mousePos.x - 2 * client.radius) / client.spacing;

        size_t dataSent;
        sf::TcpSocket::Status sendStatus;
        message = std::to_string(clickedCol);
        do {
          sendStatus = socket.send(message.data(), MAX_MESSAGE_LENGTH, dataSent);
        } while (sendStatus == sf::Socket::Status::Partial);
      }
    }

    if (status == Mat::client::Status::CONNECTED) {
      //message receiving
      size_t actuallyReceived;
      //std::cout << "trying index\n";
      //index receiving
      auto receivedStatus = socket.receive(received_instruction_index.data(), MAX_MESSAGE_LENGTH, actuallyReceived);
      if (receivedStatus == sf::Socket::Status::Done) {
        received_instruction_index.resize(actuallyReceived);  // Trim to actual length

        // Convert index to an integer
        int actualNumber = std::stoi(received_instruction_index);

        //std::cout << "Index received: " << actualNumber << std::endl;

        // Receive player ID
        int receivedPlayer;
        size_t receivedDataCount;
        sf::TcpSocket::Status
            receivePlayerStatus = socket.receive(&receivedPlayer, sizeof(receivedPlayer), receivedDataCount);

        if (receivePlayerStatus == sf::Socket::Status::Done) {
          //std::cout << "Player received: " << receivedPlayer << std::endl;

          // Process the move in the game
          client.playground_.InsertCoin(static_cast<power_of_four::TileState>(receivedPlayer), actualNumber);
        } else {
          std::cerr << "Error receiving player ID\n";
        }
      }

      //std::cout << "local port\n";
      if (socket.getLocalPort() == 0) {
        status = Mat::client::Status::NOT_CONNECTED;
      }
    }
    ImGui::SFML::Update(window, deltaClock.restart());
    auto [x, y] = window.getSize();
    ImGui::SetNextWindowSize({(float) x, (float) y}, ImGuiCond_Always);
    ImGui::SetNextWindowPos({0.0f, 0.0f}, ImGuiCond_Always);
    ImGui::Begin("Simple Chat", nullptr, ImGuiWindowFlags_NoTitleBar);
    switch (status) {
      case Mat::client::Status::NOT_CONNECTED: {
        ImGui::InputText("Host Address", serverAddress.data(), serverAddress.size());
        ImGui::SameLine();
        ImGui::Text("%hd", portNumber);
        if (ImGui::Button("Connect")) {
          if (auto address = sf::IpAddress::resolve(serverAddress)) {
            socket.setBlocking(true);
            const auto connectionStatus = socket.connect(address.value(), portNumber);
            switch (connectionStatus) {
              case sf::Socket::Status::Done:status = Mat::client::Status::CONNECTED;
                break;
              case sf::Socket::Status::NotReady:std::cerr << "Socket not ready\n";
                break;
              case sf::Socket::Status::Partial:std::cerr << "Partial\n";
                break;
              case sf::Socket::Status::Disconnected:std::cerr << "Socket disconnected\n";
                break;
              case sf::Socket::Status::Error:std::cerr << "Socket error\n";
                break;
            }
            socket.setBlocking(false);
          }
        }
        ImGui::End();
        ImGui::SFML::Render(window);
        break;
      }

      case Mat::client::Status::CONNECTED: {
        ImGui::End();
        ImGui::SFML::Render(window);
        //who wins
//        if (final_result == power_of_four::TileState::Player1 && !has_anyone_won) {
//          size_t dataSent;
//          sf::TcpSocket::Status sendStatus;
//          sendMessage = "player one wins";
//          do {
//            sendStatus = socket.send(sendMessage.data(), MAX_MESSAGE_LENGTH, dataSent);
//          } while (sendStatus == sf::Socket::Status::Partial);
//          has_anyone_won = true;
//        }
//        for (const auto &message : receivedMessages) {
//          ImGui::Text("Received message: %s", message.data());
//        }
//        if (final_result == power_of_four::TileState::Player2 && !has_anyone_won) {
//          size_t dataSent;
//          sf::TcpSocket::Status sendStatus;
//          sendMessage = "player two wins";
//          do {
//            sendStatus = socket.send(sendMessage.data(), MAX_MESSAGE_LENGTH, dataSent);
//          } while (sendStatus == sf::Socket::Status::Partial);
//          has_anyone_won = true;
//        }
//        for (const auto &message : receivedMessages) {
//          ImGui::Text("Received message: %s", message.data());
//        }

        window.clear(sf::Color(192, 0, 192, 255));  // Clear with background color
        //where things are done (colors)
        for (int row = 0; row < power_of_four::kNumberOfRows; row++) {
          for (int col = 0; col < power_of_four::kNumberOfColumns; col++) {
            int index = row * power_of_four::kNumberOfColumns + col;  // Get index

            // Set position for circle in grid layout
            circle.setPosition(sf::Vector2f(static_cast<float>(col) * static_cast<float>(client.spacing) +
                                                static_cast<float>(client.radius) * 2,
                                            static_cast<float>(row) * static_cast<float>(client.spacing) +
                                                static_cast<float>(client.radius) * 2));

            // Set color based on tile state
            if (client.playground_.ReturnState(index) == power_of_four::TileState::Player1) {
              circle.setFillColor(sf::Color(190, 0, 0, 255));
            } else if (client.playground_.ReturnState(index) == power_of_four::TileState::Player2) {
              circle.setFillColor(sf::Color(0, 0, 190, 255));
            } else if (client.playground_.ReturnState(index) == power_of_four::TileState::Player3) {
              circle.setFillColor(sf::Color(0, 190, 0, 255));
            } else if (client.playground_.ReturnState(index) == power_of_four::TileState::Player4) {
              circle.setFillColor(sf::Color(190, 190, 0, 255));
            } else {
              circle.setFillColor(sf::Color(80, 80, 80, 255));  // Empty tile
            }
            window.draw(circle);  // Draw each circle
          }

        }

        break;
      }
    }

    window.display();
  }
  ImGui::SFML::Shutdown();
  return 0;
}