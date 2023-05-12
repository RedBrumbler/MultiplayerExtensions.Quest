#include "Players/MpexPlayerManager.hpp"
#include "config.hpp"

#include "custom-types/shared/delegate.hpp"
#include "System/Action_1.hpp"
DEFINE_TYPE(MultiplayerExtensions::Players, MpexPlayerManager);

namespace MultiplayerExtensions::Players {
    void MpexPlayerManager::ctor(MultiplayerCore::Networking::MpPacketSerializer* packetSerializer, Utilities::SessionManagerEventPassthrough* sessionManagerEvents) {
        INVOKE_CTOR();

        _packetSerializer = packetSerializer;
        _sessionManagerEvents = sessionManagerEvents;
        _playerData = MpexPlayerDataDict::New_ctor();
    }

    void MpexPlayerManager::Initialize() {
        _packetSerializer->RegisterCallback<MpexPlayerData*>(
            std::bind(&MpexPlayerManager::HandlePlayerData, this, std::placeholders::_1, std::placeholders::_2)
        );


        _sessionManagerEvents->PlayerConnectedEvent += {&MpexPlayerManager::HandlePlayerConnected, this};
    }

    void MpexPlayerManager::Dispose() {
        _packetSerializer->UnregisterCallback<MpexPlayerData*>();
        _sessionManagerEvents->PlayerConnectedEvent -= {&MpexPlayerManager::HandlePlayerConnected, this};
    }

    void MpexPlayerManager::HandlePlayerConnected(GlobalNamespace::IConnectedPlayer* player) {
        auto packet = MpexPlayerData::New_ctor();
        packet->playerColor = config.playerColor;
        _packetSerializer->Send(packet);
    }

    void MpexPlayerManager::HandlePlayerData(MpexPlayerData* packet, GlobalNamespace::IConnectedPlayer* player) {
        _playerData->set_Item(player->get_userId(), packet);
        PlayerConnectedEvent.invoke(player, packet);
    }

    bool MpexPlayerManager::TryGetPlayer(const std::string& userId, MpexPlayerData*& player) {
        if (_playerData->ContainsKey(userId)) {
            player = _playerData->get_Item(userId);
            return true;
        }
        return false;
    }

    MpexPlayerData* MpexPlayerManager::GetPlayer(const std::string& userId) {
        MpexPlayerData* player = nullptr;
        TryGetPlayer(userId, player);
        return player;
    }
}
