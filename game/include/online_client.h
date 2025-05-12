//
// Created by Mat on 5/10/2025.
//

#ifndef MICROMACHINE_GAME_INCLUDE_ONLINE_CLIENT_H_
#define MICROMACHINE_GAME_INCLUDE_ONLINE_CLIENT_H_

#include <unordered_map>
#include "vec2.h"
#include "network.h"
#include "client_interface.h"
#include "inc/RoomOptions.h"

namespace micromachine {

class MyClient : public micromachine::ClientInterface {
 private:
  sf::Vector2f direction_1{};
  sf::Vector2f direction_2{};
  int local_player_nr_ = -1;

 public:
  // ✅ Méthodes obligatoires de Listener
  void debugReturn(int /*debugLevel*/, const ExitGames::Common::JString & /*string*/) override {}

  void connectionErrorReturn(int /*errorCode*/) override {}

  void clientErrorReturn(int /*errorCode*/) override {}

  void warningReturn(int /*warningCode*/) override {}

  void serverErrorReturn(int /*errorCode*/) override {}

  void joinRoomEventAction(int /*playerNr*/,
                           const ExitGames::Common::JVector<int> & /*playernrs*/,
                           const ExitGames::LoadBalancing::Player & /*player*/) override {}

  void leaveRoomEventAction(int /*playerNr*/, bool /*isInactive*/) override {}

  void joinRoomReturn(int /*localPlayerNr*/,
                      const ExitGames::Common::JVector<int> & /*playernrs*/,
                      const ExitGames::LoadBalancing::Player & /*player*/) {}

  void leaveRoomReturn(int /*errorCode*/, const ExitGames::Common::JString & /*errorString*/) override {}

  void connectReturn(int errorCode,
                     const ExitGames::Common::JString &errorString,
                     const ExitGames::Common::JString &cluster) {
    std::cout << "Connected to Photon Cloud!\n";
    ExitGames::LoadBalancing::RoomOptions roomOptions;
    micromachine::NetworkManager::GetLoadBalancingClient().opJoinOrCreateRoom(L"test-room", roomOptions);
  }

  void disconnectReturn() override {
    std::cout << "Disconnected from Photon Cloud.\n";
  }

  void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object &eventContent) override {
    //std::cout << "[Photon] customEventAction triggered!\n";
    if (eventCode == 1) {
      auto message = ExitGames::Common::ValueObject<ExitGames::Common::JString>(eventContent).getDataCopy();
      std::cout << "[Photon] Message from player " << playerNr << ": " << message.UTF8Representation().cstr()
                << std::endl;
    }
    if (eventCode == 2)
    {
      auto message = ExitGames::Common::ValueObject<ExitGames::Common::JString>(eventContent).getDataCopy();
      DecryptMess(playerNr, message);
    }
  }

  void roomListUpdate(const ExitGames::Common::JVector<ExitGames::LoadBalancing::Room> & /*roomList*/) {}

  void roomPropertiesChange(const ExitGames::Common::Hashtable & /*properties*/) {}

  void playerPropertiesChange(int /*playerNr*/, const ExitGames::Common::Hashtable & /*properties*/) {}

  void connectReturn(int errorCode,
                     const ExitGames::Common::JString &errorString,
                     const ExitGames::Common::JString &region,
                     const ExitGames::Common::JString &cluster) override {

    std::cout << "[Photon] connectReturn code: " << errorCode << ", message: "
              << errorString.UTF8Representation().cstr() << "\n";
    if (errorCode == 0) {
      std::cout << "[Photon] Connected successfully! Joining room...\n";
      ExitGames::LoadBalancing::RoomOptions roomOptions;
      micromachine::NetworkManager::GetLoadBalancingClient().opJoinOrCreateRoom(L"test-room", roomOptions);
    }
  }

  static int getLocalPlayerNr() {
    return micromachine::NetworkManager::GetLoadBalancingClient()
        .getLocalPlayer()
        .getNumber();
  }

  [[nodiscard]] static std::vector<int> getAllPlayerNrs() {
    std::vector<int> result;
    // Accède à la room jointe
    const auto& room = micromachine::NetworkManager::GetLoadBalancingClient()
        .getCurrentlyJoinedRoom();
    // Photon renvoie un JVector de pointeurs Player*
    ExitGames::Common::JVector<ExitGames::LoadBalancing::Player*> players = room.getPlayers();
    // Itère dessus
    for (unsigned int i = 0; i < players.getSize(); ++i) {
      result.push_back(players[i]->getNumber());
    }
    return result;
  }


  static int getRemotePlayerNr() {
    auto all = getAllPlayerNrs();
    int local = getLocalPlayerNr();
    for(int n : all)
      if(n != local) return n;
    return -1;
  }

  void DecryptMess(int playerNr, const ExitGames::Common::JString& message){
    std::string dirStr = message.UTF8Representation().cstr();
    size_t commaPos = dirStr.find(',');
    float directionX = 0.0f;
    float directionY = 0.0f;
    if (commaPos != std::string::npos) {
      std::string xStr = dirStr.substr(0, commaPos);
      std::string yStr = dirStr.substr(commaPos + 1);

      directionX = std::stof(xStr);
      directionY = std::stof(yStr);
    }
    const auto direction = sf::Vector2f {directionX,directionY};
    if (playerNr==1) {
      direction_1 = direction;
    }

    if (playerNr==2) {
      direction_2 = direction;
    }
  }
  sf::Vector2f& direction1() {return direction_1;}
  sf::Vector2f& direction2() {return direction_2;}
};


}

#endif //MICROMACHINE_GAME_INCLUDE_ONLINE_CLIENT_H_
