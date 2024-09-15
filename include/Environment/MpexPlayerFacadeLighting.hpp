#pragma once

#include "custom-types/shared/macros.hpp"
#include "lapiz/shared/macros.hpp"

#include "UnityEngine/MonoBehaviour.hpp"
#include "GlobalNamespace/IConnectedPlayer.hpp"
#include "GlobalNamespace/IMultiplayerSessionManager.hpp"
#include "GlobalNamespace/MenuLightsManager.hpp"
#include "GlobalNamespace/TubeBloomPrePassLight.hpp"
#include "System/Action_1.hpp"


#include "GlobalNamespace/ColorSO.hpp"
#include "GlobalNamespace/LightsAnimator.hpp"
#include "GlobalNamespace/MultiplayerController.hpp"
#include "GlobalNamespace/IScoreSyncStateManager.hpp"
#include "GlobalNamespace/MultiplayerLeadPlayerProvider.hpp"
#include "GlobalNamespace/MultiplayerGameplayAnimator.hpp"
#include "GlobalNamespace/MultiplayerSyncState_3.hpp"

namespace MultiplayerExtensions::Environment {
    using MultiplayerSyncState = ::GlobalNamespace::MultiplayerSyncState_3<GlobalNamespace::StandardScoreSyncState, GlobalNamespace::StandardScoreSyncState::Score, int>;

}

DECLARE_CLASS_CODEGEN(MultiplayerExtensions::Environment, MpexPlayerFacadeLighting, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD_PRIVATE(GlobalNamespace::IConnectedPlayer*, _connectedPlayer);
    DECLARE_INSTANCE_FIELD_PRIVATE(GlobalNamespace::MultiplayerController*, _multiplayerController);
    DECLARE_INSTANCE_FIELD_PRIVATE(GlobalNamespace::IScoreSyncStateManager*, _scoreProvider);
    DECLARE_INSTANCE_FIELD_PRIVATE(GlobalNamespace::MultiplayerLeadPlayerProvider*, _leadPlayerProvider);

    DECLARE_INSTANCE_FIELD_PRIVATE(GlobalNamespace::MultiplayerGameplayAnimator*, _gameplayAnimator);
    DECLARE_INSTANCE_FIELD_PRIVATE(MultiplayerSyncState*, _syncState);

    DECLARE_INSTANCE_FIELD_PRIVATE(System::Action_1<StringW>*, _handleLeaderSelectedAction);
    DECLARE_INSTANCE_FIELD_PRIVATE(bool, _isLeading);
    DECLARE_INSTANCE_FIELD_PRIVATE(int, _highestCombo);

    DECLARE_INJECT_METHOD(void, Inject, GlobalNamespace::IConnectedPlayer* connectedPlayer, GlobalNamespace::MultiplayerController* multiplayerController, GlobalNamespace::IScoreSyncStateManager* scoreProvider, GlobalNamespace::MultiplayerLeadPlayerProvider* leadPlayerProvider);

    DECLARE_INSTANCE_METHOD(void, Update);
    DECLARE_INSTANCE_METHOD(void, OnEnable);
    DECLARE_INSTANCE_METHOD(void, OnDisable);

    public:
        void SetLights(UnityEngine::Color color);
    private:
        void HandleNewLeaderWasSelected(StringW userId);

        ArrayW<UnityW<GlobalNamespace::LightsAnimator>> get_allLights();
        ArrayW<UnityW<GlobalNamespace::LightsAnimator>> get_gameplayLights();

        GlobalNamespace::ColorSO* get_activeColor();
        GlobalNamespace::ColorSO* get_leadingColor();
        GlobalNamespace::ColorSO* get_failedColor();
)
