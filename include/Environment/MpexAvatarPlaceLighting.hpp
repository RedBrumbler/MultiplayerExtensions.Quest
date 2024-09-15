#pragma once

#include "custom-types/shared/macros.hpp"
#include "lapiz/shared/macros.hpp"

#include "UnityEngine/MonoBehaviour.hpp"
#include "GlobalNamespace/IConnectedPlayer.hpp"
#include "GlobalNamespace/IMultiplayerSessionManager.hpp"
#include "GlobalNamespace/MenuLightsManager.hpp"
#include "GlobalNamespace/TubeBloomPrePassLight.hpp"

#include "multiplayer-core/shared/Players/MpPlayerManager.hpp"
#include "Players/MpexPlayerManager.hpp"
#include "Players/MpexPlayerData.hpp"
#include "Utilities/SessionManagerEventPassthrough.hpp"

#include "sombrero/shared/ColorUtils.hpp"

struct Hook_MultiplayerLobbyAvatarPlaceManager_SpawnAllPlaces;
namespace MultiplayerExtensions::Patchers {
    class AvatarPlacePatcher;
}
DECLARE_CLASS_CODEGEN(MultiplayerExtensions::Environment, MpexAvatarPlaceLighting, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD_PRIVATE(GlobalNamespace::IMultiplayerSessionManager*, _sessionManager);
    DECLARE_INSTANCE_FIELD_PRIVATE(Utilities::SessionManagerEventPassthrough*, _sessionManagerEvents);
    DECLARE_INSTANCE_FIELD_PRIVATE(GlobalNamespace::MenuLightsManager*, _lightsManager);
    DECLARE_INSTANCE_FIELD_PRIVATE(Players::MpexPlayerManager*, _mpexPlayerManager);

    DECLARE_INSTANCE_FIELD_PRIVATE_DEFAULT(ListW<GlobalNamespace::TubeBloomPrePassLight*>, _lights, nullptr);
    DECLARE_INSTANCE_FIELD_PRIVATE(Sombrero::FastColor, _targetColor);
    DECLARE_INSTANCE_FIELD_PRIVATE(int, _sortIndex);

    DECLARE_INJECT_METHOD(void, Inject, GlobalNamespace::IMultiplayerSessionManager* sessionManager, Utilities::SessionManagerEventPassthrough* sessionManagerEvents, GlobalNamespace::MenuLightsManager* menuLightsManager, Players::MpexPlayerManager* mpexPlayerManager);

    DECLARE_INSTANCE_METHOD(void, Start);
    DECLARE_INSTANCE_METHOD(void, Update);
    DECLARE_INSTANCE_METHOD(void, OnEnable);
    DECLARE_INSTANCE_METHOD(void, OnDisable);

    DECLARE_CTOR(ctor);
    public:
        static inline const float SMOOTH_TIME = 2.0f;
        void HandlePlayerData(GlobalNamespace::IConnectedPlayer* player, Players::MpexPlayerData* data);
        void HandlePlayerConnected(GlobalNamespace::IConnectedPlayer* player);
        void HandlePlayerDisconnected(GlobalNamespace::IConnectedPlayer* player);

        UnityEngine::Color get_targetColor() const;
        void SetColor(UnityEngine::Color color, bool immedate);
        UnityEngine::Color GetColor();
    private:
        friend ::Hook_MultiplayerLobbyAvatarPlaceManager_SpawnAllPlaces;
        friend ::MultiplayerExtensions::Patchers::AvatarPlacePatcher;
        void SetColor(UnityEngine::Color color);
)
