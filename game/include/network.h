﻿//
// Created by Mat on 5/6/2025.
//

#ifndef MICROMACHINE_GAME_INCLUDE_NETWORK_H_
#define MICROMACHINE_GAME_INCLUDE_NETWORK_H_

#include "client_interface.h"
#include "LoadBalancing-cpp/inc/Listener.h"
#include "LoadBalancing-cpp/inc/ClientConstructOptions.h"
#include "LoadBalancing-cpp/inc/Client.h"
#include <memory>
#include <iostream>

namespace micromachine {

class NetworkManager {
 public:
  //Démarre le client Photon avec l’interface utilisateur et les options de construction.
  static void Begin(ClientInterface *client, const ExitGames::LoadBalancing::ClientConstructOptions &clientConstructOptions);

  //Fait avancer le client Photon (doit être appelé régulièrement dans la boucle principale).
  static void Tick();

  //Déconnecte proprement le client Photon.
  static void End();

  //Permet d'accéder au client Photon actif.
  static ExitGames::LoadBalancing::Client &GetLoadBalancingClient();
};
}

#endif //MICROMACHINE_GAME_INCLUDE_NETWORK_H_
