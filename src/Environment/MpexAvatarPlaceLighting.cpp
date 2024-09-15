#include "Environment/MpexAvatarPlaceLighting.hpp"
#include "config.hpp"

#include "UnityEngine/Time.hpp"

using namespace System::Collections::Generic;

DEFINE_TYPE(MultiplayerExtensions::Environment, MpexAvatarPlaceLighting);

static constexpr inline UnityEngine::Color lerp(UnityEngine::Color a, UnityEngine::Color b, float t) {
    return {
        a.r + (b.r - a.r) * t,
        a.g + (b.g - a.g) * t,
        a.b + (b.b - a.b) * t,
        a.a + (b.a - a.a) * t,
    };
}

namespace MultiplayerExtensions::Environment {
    void MpexAvatarPlaceLighting::ctor() {
        INVOKE_CTOR();
        INVOKE_BASE_CTOR(classof(UnityEngine::MonoBehaviour*));
        _lights = ListW<GlobalNamespace::TubeBloomPrePassLight*>::New();
    }

    void MpexAvatarPlaceLighting::Inject(GlobalNamespace::IMultiplayerSessionManager* sessionManager, Utilities::SessionManagerEventPassthrough* sessionManagerEvents, GlobalNamespace::MenuLightsManager* menuLightsManager, Players::MpexPlayerManager* mpexPlayerManager) {
        _sessionManager = sessionManager;
        _sessionManagerEvents = sessionManagerEvents;
        _lightsManager = menuLightsManager;
        _mpexPlayerManager = mpexPlayerManager;
    }

    void MpexAvatarPlaceLighting::Start() {
        auto ls = GetComponentsInChildren<GlobalNamespace::TubeBloomPrePassLight*>();
        _lights->EnsureCapacity(ls.size());
        for (auto l : ls) _lights->Add(l);

        if (!_sessionManager || !_sessionManagerEvents || !_lightsManager || !_mpexPlayerManager || !_sessionManager->get_localPlayer()) return;

        if (_sessionManager->get_localPlayer()->get_sortIndex() == _sortIndex) {
            SetColor(config.playerColor, true);
            return;
        }

        for(auto player : ListW<GlobalNamespace::IConnectedPlayer*>(_sessionManager->get_connectedPlayers())) {
            if (player->get_sortIndex() == _sortIndex) {
                Players::MpexPlayerData* mpexPlayer = nullptr;
                SetColor(_mpexPlayerManager->TryGetPlayer(player->get_userId(), mpexPlayer) ? mpexPlayer->playerColor : Config::defaultPlayerColor, true);
                break;
            }
        }
    }

    void MpexAvatarPlaceLighting::OnEnable() {
        _mpexPlayerManager->PlayerConnectedEvent += {&MpexAvatarPlaceLighting::HandlePlayerData, this};
        _sessionManagerEvents->PlayerConnectedEvent += {&MpexAvatarPlaceLighting::HandlePlayerConnected, this};
        _sessionManagerEvents->PlayerDisconnectedEvent += {&MpexAvatarPlaceLighting::HandlePlayerDisconnected, this};
    }

    void MpexAvatarPlaceLighting::OnDisable() {
        _mpexPlayerManager->PlayerConnectedEvent -= {&MpexAvatarPlaceLighting::HandlePlayerData, this};
        _sessionManagerEvents->PlayerConnectedEvent -= {&MpexAvatarPlaceLighting::HandlePlayerConnected, this};
        _sessionManagerEvents->PlayerDisconnectedEvent -= {&MpexAvatarPlaceLighting::HandlePlayerDisconnected, this};
    }

    void MpexAvatarPlaceLighting::HandlePlayerData(GlobalNamespace::IConnectedPlayer* player, Players::MpexPlayerData* data) {
        if (player->get_sortIndex() == _sortIndex)
            SetColor(data->playerColor, false);
    }

    void MpexAvatarPlaceLighting::HandlePlayerConnected(GlobalNamespace::IConnectedPlayer* player) {
        if (player->get_sortIndex() != _sortIndex) return;
        Players::MpexPlayerData* data = nullptr;
        if (_mpexPlayerManager->TryGetPlayer(player->get_userId(), data))
            SetColor(data->playerColor, false);
        else
            SetColor(Config::defaultPlayerColor, false);
    }

    void MpexAvatarPlaceLighting::HandlePlayerDisconnected(GlobalNamespace::IConnectedPlayer* player) {
        if (player->get_sortIndex() == _sortIndex)
            SetColor({}, false);
    }

    void MpexAvatarPlaceLighting::Update() {
        auto current = static_cast<Sombrero::FastColor>(GetColor());
        if (current == _targetColor) return;
        if (_lightsManager->IsColorVeryCloseToColor(current, _targetColor))
            SetColor(_targetColor);
        else
            SetColor(lerp(current, _targetColor, UnityEngine::Time::get_deltaTime() * SMOOTH_TIME));
    }

    void MpexAvatarPlaceLighting::SetColor(UnityEngine::Color color, bool immedate) {
        if (immedate)
            SetColor(color);
    }
    UnityEngine::Color MpexAvatarPlaceLighting::GetColor() {
        if (_lights.size() > 0)
            return _lights[0]->get_color();
        return {};
    }
    void MpexAvatarPlaceLighting::SetColor(UnityEngine::Color color) {
        for (auto light : _lights) {
            light->set_color(color);
            light->Refresh();
        }
    }
    UnityEngine::Color MpexAvatarPlaceLighting::get_targetColor() const { return _targetColor; }
}
