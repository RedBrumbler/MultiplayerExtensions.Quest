#pragma once

#include "custom-types/shared/macros.hpp"

#include "GlobalNamespace/IMultiplayerSessionManager.hpp"
#include "GlobalNamespace/IConnectedPlayer.hpp"

DECLARE_CLASS_CODEGEN(MultiplayerExtensions::Utilities, SessionManagerEventPassthrough, System::Object,
    DECLARE_INSTANCE_FIELD_PRIVATE(GlobalNamespace::IMultiplayerSessionManager*, _sessionManager);

    DECLARE_CTOR(ctor, GlobalNamespace::IMultiplayerSessionManager* sessionManager);

    public:
        UnorderedEventCallback<GlobalNamespace::IConnectedPlayer*> PlayerConnectedEvent;
        UnorderedEventCallback<GlobalNamespace::IConnectedPlayer*> PlayerDisconnectedEvent;
    private:
        void InvokePlayerConnectedEvent(GlobalNamespace::IConnectedPlayer* player);
        void InvokePlayerDisconnectedEvent(GlobalNamespace::IConnectedPlayer* player);
)
