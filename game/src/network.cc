//
// Created by Mat on 5/6/2025.
//


#include "network.h"

//static const ExitGames::Common::JString appID = L"xxxxxxxxxxxxxxxxxxxxxxx"; // set your app id here
//static const ExitGames::Common::JString appVersion = L"1.0";
//
//
//static std::unique_ptr<ExitGames::LoadBalancing::Client> loadBalancingClient_;
//
//
//void NetworkManager::Begin(ClientInterface* client, const ExitGames::LoadBalancing::ClientConstructOptions& clientConstructOptions)
//{
//// connect() is asynchronous -
//// the actual result arrives in the Listener::connectReturn()
//// or the Listener::connectionErrorReturn() callback
//  loadBalancingClient_ = std::make_unique<ExitGames::LoadBalancing::Client>(*client, appID, appVersion, clientConstructOptions);
//  ExitGames::LoadBalancing::ConnectOptions connectOptions_{};
//  if(!loadBalancingClient_->connect(connectOptions_))
//  {
//    std::cerr << "Could not connect. \n";
//  }
//}
//
//void NetworkManager::Tick()
//{
//  loadBalancingClient_->service();
//}
//
//void NetworkManager::End()
//{
//  loadBalancingClient_->disconnect();
//}
//ExitGames::LoadBalancing::Client& GetLoadBalancingClient()
//{
//  return *loadBalancingClient_;
//}
//}