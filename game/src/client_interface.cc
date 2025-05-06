//
// Created by Mat on 5/6/2025.
//

#include "client_interface.h"

namespace micromachine
{

void ClientInterface::connectReturn(int errorCode, const ExitGames::Common::JString& errorString,
                                    const ExitGames::Common::JString& region, const ExitGames::Common::JString& cluster)
{
  if (errorCode == 0)
    std::cout << "Connected to Photon successfully! Region: " << region.UTF8Representation().cstr() << "\n";
  else
    std::cerr << "Failed to connect: " << errorString.UTF8Representation().cstr() << "\n";
}

void ClientInterface::disconnectReturn()
{
  std::cout << "Disconnected from Photon.\n";
}

void ClientInterface::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
{
  std::cout << "Received custom event. Player: " << playerNr << ", Event Code: " << static_cast<int>(eventCode) << "\n";
  // Vous pouvez parser eventContent ici
}

void ClientInterface::operationReturn(const ExitGames::Photon::OperationResponse& operationResponse)
{
  std::cout << "Operation returned. Code: " << operationResponse.getOperationCode() << ", Return Code: " << operationResponse.getReturnCode() << "\n";
}

}
