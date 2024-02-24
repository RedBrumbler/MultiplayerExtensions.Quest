#include "Environment/MpexConnectedObjectManager.hpp"
#include "config.hpp"

#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"
#include "custom-types/shared/delegate.hpp"

DEFINE_TYPE(MultiplayerExtensions::Environment, MpexConnectedObjectManager);

namespace MultiplayerExtensions::Environment {
    void MpexConnectedObjectManager::Inject(GlobalNamespace::MultiplayerConnectedPlayerSpectatingSpot* playerSpectatingSpot, GlobalNamespace::IConnectedPlayerBeatmapObjectEventManager* beatmapObjectEventManager, GlobalNamespace::BeatmapObjectManager* beatmapObjectManager) {
        _playerSpectatingSpot = playerSpectatingSpot;
        _beatmapObjectEventManager = beatmapObjectEventManager;
        _beatmapObjectManager = beatmapObjectManager;
    }

    void MpexConnectedObjectManager::Start() {
        _observedChangedEvent = custom_types::MakeDelegate<System::Action_1<bool>*>(
            std::function<void(bool)>(
                std::bind(&MpexConnectedObjectManager::HandleIsObservedChangedEvent, this, std::placeholders::_1)
            )
        );

        _playerSpectatingSpot->add_isObservedChangedEvent(_observedChangedEvent);

        if (config.disableMultiplayerObjects)
            _beatmapObjectEventManager->Pause();
    }

    void MpexConnectedObjectManager::OnDestroy() {
        if (_playerSpectatingSpot && _playerSpectatingSpot->m_CachedPtr)
            _playerSpectatingSpot->remove_isObservedChangedEvent(_observedChangedEvent);
    }

    void MpexConnectedObjectManager::HandleIsObservedChangedEvent(bool isObserved) {
        if (config.disableMultiplayerPlatforms)
            get_transform()->Find("Construction")->get_gameObject()->SetActive(isObserved);
        if (config.disableMultiplayerLights)
            get_transform()->Find("Lasers")->get_gameObject()->SetActive(isObserved);
        if (!config.disableMultiplayerObjects) return;
        if (isObserved) {
            _beatmapObjectEventManager->Resume();
            return;
        }

        _beatmapObjectEventManager->Pause();
        _beatmapObjectManager->DissolveAllObjects();

    }
}
