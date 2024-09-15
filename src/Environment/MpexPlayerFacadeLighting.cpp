#include "Environment/MpexPlayerFacadeLighting.hpp"
#include "config.hpp"

#include "GlobalNamespace/StandardScoreSyncState.hpp"
#include "GlobalNamespace/StandardScoreSyncStateNetSerializable.hpp"
#include "GlobalNamespace/StandardScoreSyncStateDeltaNetSerializable.hpp"
#include "GlobalNamespace/IScoreSyncStateManager_5.hpp"
#include "GlobalNamespace/ConnectedPlayerHelpers.hpp"
#include "custom-types/shared/delegate.hpp"

DEFINE_TYPE(MultiplayerExtensions::Environment, MpexPlayerFacadeLighting);

static constexpr inline UnityEngine::Color lerp(UnityEngine::Color a, UnityEngine::Color b, float t) {
    return {
        a.r + (b.r - a.r) * t,
        a.g + (b.g - a.g) * t,
        a.b + (b.b - a.b) * t,
        a.a + (b.a - a.a) * t,
    };
}

namespace MultiplayerExtensions::Environment {
    void MpexPlayerFacadeLighting::Inject(GlobalNamespace::IConnectedPlayer* connectedPlayer, GlobalNamespace::MultiplayerController* multiplayerController, GlobalNamespace::IScoreSyncStateManager* scoreProvider, GlobalNamespace::MultiplayerLeadPlayerProvider* leadPlayerProvider) {
        _connectedPlayer = connectedPlayer;
        _multiplayerController = multiplayerController;
        _scoreProvider = scoreProvider;
        _leadPlayerProvider = leadPlayerProvider;
    }

    void MpexPlayerFacadeLighting::OnEnable() {
        _gameplayAnimator = GetComponentInChildren<GlobalNamespace::MultiplayerGameplayAnimator*>();
        _syncState = _scoreProvider->i___GlobalNamespace__IScoreSyncStateManager_5___GlobalNamespace__StandardScoreSyncState___GlobalNamespace____StandardScoreSyncState__Score_int32_t___GlobalNamespace__StandardScoreSyncStateNetSerializable____GlobalNamespace__StandardScoreSyncStateDeltaNetSerializable__()->GetSyncStateForPlayer(_connectedPlayer);
        _handleLeaderSelectedAction = custom_types::MakeDelegate<System::Action_1<StringW>*>(
            std::function<void(StringW)>(
                std::bind(&MpexPlayerFacadeLighting::HandleNewLeaderWasSelected, this, std::placeholders::_1)
            )
        );
        _leadPlayerProvider->add_newLeaderWasSelectedEvent(_handleLeaderSelectedAction);
    }

    void MpexPlayerFacadeLighting::OnDisable() {
        _leadPlayerProvider->remove_newLeaderWasSelectedEvent(_handleLeaderSelectedAction);
    }

    void MpexPlayerFacadeLighting::Update() {
        using namespace GlobalNamespace;
        if (_multiplayerController->get_state() == MultiplayerController::State::Gameplay &&
            !ConnectedPlayerHelpers::IsFailed(_connectedPlayer)) {
                auto combo = _syncState->GetState(StandardScoreSyncState::Score::Combo, _syncState->get_player()->get_offsetSyncTime());
                if (combo > _highestCombo)
                    _highestCombo = combo;

                auto baseColor = _isLeading ? get_leadingColor() : get_activeColor();
                float failPercentage = (std::min((float)_highestCombo, 20.0f) - (float)combo) / 20.0f;
                auto color = config.missColor;
                SetLights(lerp(baseColor->get_color(), color, failPercentage));
        }
    }

    void MpexPlayerFacadeLighting::HandleNewLeaderWasSelected(StringW userId) {
        _isLeading = userId == _connectedPlayer->get_userId();
    }

    void MpexPlayerFacadeLighting::SetLights(UnityEngine::Color color) {
        for (auto light : get_gameplayLights())
            light->SetColor(color);
    }

    ArrayW<UnityW<GlobalNamespace::LightsAnimator>> MpexPlayerFacadeLighting::get_allLights() {
        return _gameplayAnimator->_allLightsAnimators;
    }
    ArrayW<UnityW<GlobalNamespace::LightsAnimator>> MpexPlayerFacadeLighting::get_gameplayLights() {
        return _gameplayAnimator->_gameplayLightsAnimators;
    }
    GlobalNamespace::ColorSO* MpexPlayerFacadeLighting::get_activeColor() {
        return _gameplayAnimator->_activeLightsColor;
    }
    GlobalNamespace::ColorSO* MpexPlayerFacadeLighting::get_leadingColor() {
        return _gameplayAnimator->_leadingLightsColor;
    }
    GlobalNamespace::ColorSO* MpexPlayerFacadeLighting::get_failedColor() {
        return _gameplayAnimator->_failedLightsColor;
    }
}
