﻿//
// Created by Mat on 5/6/2025.
//


#include "network.h"
#include "LoadBalancing-cpp/inc/ConnectOptions.h"

namespace micromachine {
static const ExitGames::Common::JString appID = L"4d62da01-f896-4dbc-9f39-3a722de05d1e"; // set your app id here
static const ExitGames::Common::JString appVersion = L"1.0";

static std::unique_ptr<ExitGames::LoadBalancing::Client> loadBalancingClient_;

void NetworkManager::Begin(ClientInterface *client,
                           const ExitGames::LoadBalancing::ClientConstructOptions &clientConstructOptions) {
// connect() is asynchronous -
// the actual result arrives in the Listener::connectReturn()
// or the Listener::connectionErrorReturn() callback
  loadBalancingClient_ =
      std::make_unique<ExitGames::LoadBalancing::Client>(*client, appID, appVersion, clientConstructOptions);
  ExitGames::LoadBalancing::ConnectOptions connectOptions_{};
  if (!loadBalancingClient_->connect(connectOptions_)) {
    std::cerr << "Could not connect. \n";
  }
}

void NetworkManager::Tick() {
  loadBalancingClient_->service();
}

void NetworkManager::End() {
  loadBalancingClient_->disconnect();
}

ExitGames::LoadBalancing::Client &NetworkManager::GetLoadBalancingClient() {
  return *loadBalancingClient_;
}
}