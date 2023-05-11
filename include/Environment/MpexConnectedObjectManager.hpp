#pragma once

#include "custom-types/shared/macros.hpp"
#include "lapiz/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"

#include "GlobalNamespace/MultiplayerConnectedPlayerSpectatingSpot.hpp"
#include "GlobalNamespace/IConnectedPlayerBeatmapObjectEventManager.hpp"
#include "GlobalNamespace/BeatmapObjectManager.hpp"
#include "System/Action_1.hpp"

DECLARE_CLASS_CODEGEN(MultiplayerExtensions::Environment, MpexConnectedObjectManager, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD_PRIVATE(System::Action_1<bool>*, _observedChangedEvent);
    DECLARE_INSTANCE_FIELD_PRIVATE(GlobalNamespace::MultiplayerConnectedPlayerSpectatingSpot*, _playerSpectatingSpot);
    DECLARE_INSTANCE_FIELD_PRIVATE(GlobalNamespace::IConnectedPlayerBeatmapObjectEventManager*, _beatmapObjectEventManager);
    DECLARE_INSTANCE_FIELD_PRIVATE(GlobalNamespace::BeatmapObjectManager*, _beatmapObjectManager);

    DECLARE_INSTANCE_METHOD(void, Start);
    DECLARE_INSTANCE_METHOD(void, OnDestroy);
    DECLARE_INSTANCE_METHOD(void, HandleIsObservedChangedEvent, bool isObserved);

    DECLARE_INJECT_METHOD(void, Inject, GlobalNamespace::MultiplayerConnectedPlayerSpectatingSpot* playerSpectatingSpot, GlobalNamespace::IConnectedPlayerBeatmapObjectEventManager* beatmapObjectEventManager, GlobalNamespace::BeatmapObjectManager* beatmapObjectManager);
)
