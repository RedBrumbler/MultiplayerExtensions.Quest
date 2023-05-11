#pragma once

#include "custom-types/shared/macros.hpp"
#include "lapiz/shared/macros.hpp"

#include "HMUI/FlowCoordinator.hpp"
#include "HMUI/ViewController.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ViewController_AnimationType.hpp"

#include "GlobalNamespace/ILevelGameplaySetupData.hpp"
#include "GlobalNamespace/MainFlowCoordinator.hpp"
#include "GlobalNamespace/ILobbyGameStateController.hpp"
#include "System/Action_1.hpp"

#include "MpexSettingsViewController.hpp"
#include "MpexEnvironmentViewController.hpp"
#include "MpexMiscViewController.hpp"

DECLARE_CLASS_CODEGEN(MultiplayerExtensions::UI, MpexSetupFlowCoordinator, HMUI::FlowCoordinator,
    DECLARE_INSTANCE_FIELD_PRIVATE(GlobalNamespace::MainFlowCoordinator*, _mainFlowCoordinator);
    DECLARE_INSTANCE_FIELD_PRIVATE(MpexSettingsViewController*, _settingsViewController);
    DECLARE_INSTANCE_FIELD_PRIVATE(MpexEnvironmentViewController*, _environmentViewController);
    DECLARE_INSTANCE_FIELD_PRIVATE(MpexMiscViewController*, _miscViewController);
    DECLARE_INSTANCE_FIELD_PRIVATE(GlobalNamespace::ILobbyGameStateController*, _gameStateController);
    DECLARE_INSTANCE_FIELD_PRIVATE(System::Action_1<GlobalNamespace::ILevelGameplaySetupData*>*, _dismissGameStartedAction);

    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::il2cpp_type_check::MetadataGetter<&::HMUI::FlowCoordinator::DidActivate>::get(), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(void, DidDeactivate, il2cpp_utils::il2cpp_type_check::MetadataGetter<&::HMUI::FlowCoordinator::DidDeactivate>::get(), bool removedFromHierarchy, bool screenSystemDisabling);
    DECLARE_OVERRIDE_METHOD(void, BackButtonWasPressed, il2cpp_utils::il2cpp_type_check::MetadataGetter<&::HMUI::FlowCoordinator::BackButtonWasPressed>::get(), HMUI::ViewController* topViewController);

    DECLARE_INJECT_METHOD(void, Inject, GlobalNamespace::MainFlowCoordinator* mainFlowCoordinator, MpexSettingsViewController* settingsViewController, MpexEnvironmentViewController* environmentViewController, MpexMiscViewController* miscViewController, GlobalNamespace::ILobbyGameStateController* gameStateController);
    DECLARE_CTOR(ctor);
    private:
        void DismissGameStartedEvent(GlobalNamespace::ILevelGameplaySetupData* _);
)
