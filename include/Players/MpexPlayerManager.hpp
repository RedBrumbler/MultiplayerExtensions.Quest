#pragma once

#include "custom-types/shared/macros.hpp"

#include "MpexPlayerData.hpp"
#include "GlobalNamespace/IMultiplayerSessionManager.hpp"
#include "GlobalNamespace/IConnectedPlayer.hpp"
#include "Zenject/IInitializable.hpp"
#include "System/IDisposable.hpp"

#include "Utilities/SessionManagerEventPassthrough.hpp"

#include "multiplayer-core/shared/Networking/MpPacketSerializer.hpp"

#include "System/Collections/Concurrent/ConcurrentDictionary_2.hpp"

using MpexPlayerDataDict = System::Collections::Concurrent::ConcurrentDictionary_2<StringW, MultiplayerExtensions::Players::MpexPlayerData*>;
DECLARE_CLASS_CODEGEN_INTERFACES(MultiplayerExtensions::Players, MpexPlayerManager, Il2CppObject, std::vector<Il2CppClass*>({classof(Zenject::IInitializable*), classof(System::IDisposable*)}),
    DECLARE_INSTANCE_FIELD_PRIVATE(MultiplayerCore::Networking::MpPacketSerializer*, _packetSerializer);
    DECLARE_INSTANCE_FIELD_PRIVATE(Utilities::SessionManagerEventPassthrough*, _sessionManagerEvents);
    DECLARE_INSTANCE_FIELD_PRIVATE(MpexPlayerDataDict*, _playerData);

    DECLARE_OVERRIDE_METHOD_MATCH(void, Initialize, &::Zenject::IInitializable::Initialize);
    DECLARE_OVERRIDE_METHOD_MATCH(void, Dispose, &::System::IDisposable::Dispose);

    DECLARE_CTOR(ctor, MultiplayerCore::Networking::MpPacketSerializer* packetSerializer, Utilities::SessionManagerEventPassthrough* sessionManagerEvents);

    public:
        UnorderedEventCallback<GlobalNamespace::IConnectedPlayer*, MpexPlayerData*> PlayerConnectedEvent;

        bool TryGetPlayer(const std::string& userId, MpexPlayerData*& player);
        MpexPlayerData* GetPlayer(const std::string& userId);
    private:
        void HandlePlayerConnected(GlobalNamespace::IConnectedPlayer* player);
        void HandlePlayerData(MpexPlayerData* packet, GlobalNamespace::IConnectedPlayer* player);
)
