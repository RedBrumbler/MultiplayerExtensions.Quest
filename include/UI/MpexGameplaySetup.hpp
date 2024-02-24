#pragma once

#include "custom-types/shared/macros.hpp"
#include "lapiz/shared/macros.hpp"
#include "bsml/shared/macros.hpp"

#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/Transform.hpp"
#include "Zenject/IInitializable.hpp"
#include "System/IDisposable.hpp"

#include "GlobalNamespace/GameplaySetupViewController.hpp"
#include "GlobalNamespace/MainFlowCoordinator.hpp"
#include "GlobalNamespace/MultiplayerSettingsPanelController.hpp"
#include "MpexSetupFlowCoordinator.hpp"

DECLARE_CLASS_CODEGEN_INTERFACES(MultiplayerExtensions::UI, MpexGameplaySetup, System::Object, classof(Zenject::IInitializable*),
    DECLARE_INSTANCE_FIELD_PRIVATE(GlobalNamespace::GameplaySetupViewController*, _gameplaySetup);
    DECLARE_INSTANCE_FIELD_PRIVATE(GlobalNamespace::MultiplayerSettingsPanelController*, _multiplayerSettingsPanel);
    DECLARE_INSTANCE_FIELD_PRIVATE(GlobalNamespace::MainFlowCoordinator*, _mainFlowCoordinator);
    DECLARE_INSTANCE_FIELD_PRIVATE(MpexSetupFlowCoordinator*, _setupFlowCoordinator);

    DECLARE_INSTANCE_FIELD_PRIVATE(UnityEngine::Transform*, vert);

    DECLARE_INJECT_METHOD(void, Inject, GlobalNamespace::GameplaySetupViewController* gameplaySetup, GlobalNamespace::MainFlowCoordinator* mainFlowCoordinator, MpexSetupFlowCoordinator* setupFlowCoordinator);

    DECLARE_OVERRIDE_METHOD_MATCH(void, Initialize, &::Zenject::IInitializable::Initialize);

    DECLARE_INSTANCE_METHOD(void, PresentPreferences);
    DECLARE_BSML_PROPERTY(bool, soloEnvironment);
    DECLARE_DEFAULT_CTOR();
)
