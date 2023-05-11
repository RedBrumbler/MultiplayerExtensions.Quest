#include "Utilities/SessionManagerEventPassthrough.hpp"

#include "System/Action_1.hpp"
#include "custom-types/shared/delegate.hpp"

DEFINE_TYPE(MultiplayerExtensions::Utilities, SessionManagerEventPassthrough);

namespace MultiplayerExtensions::Utilities {
    void SessionManagerEventPassthrough::ctor(GlobalNamespace::IMultiplayerSessionManager* sessionManager) {
        INVOKE_CTOR();
        _sessionManager = sessionManager;

        _sessionManager->add_playerConnectedEvent(
            custom_types::MakeDelegate<System::Action_1<GlobalNamespace::IConnectedPlayer*>*>(
                std::function<void(GlobalNamespace::IConnectedPlayer*)>(
                    std::bind(&SessionManagerEventPassthrough::InvokePlayerConnectedEvent, this, std::placeholders::_1)
                )
            )
        );

        _sessionManager->add_playerDisconnectedEvent(
            custom_types::MakeDelegate<System::Action_1<GlobalNamespace::IConnectedPlayer*>*>(
                std::function<void(GlobalNamespace::IConnectedPlayer*)>(
                    std::bind(&SessionManagerEventPassthrough::InvokePlayerConnectedEvent, this, std::placeholders::_1)
                )
            )
        );
    }

    void SessionManagerEventPassthrough::InvokePlayerConnectedEvent(GlobalNamespace::IConnectedPlayer* player) {
        PlayerConnectedEvent.invoke(player);
    }

    void SessionManagerEventPassthrough::InvokePlayerDisconnectedEvent(GlobalNamespace::IConnectedPlayer* player) {
        PlayerDisconnectedEvent.invoke(player);
    }
}
