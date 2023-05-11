#pragma once

#include "custom-types/shared/macros.hpp"
#include "lapiz/shared/macros.hpp"
#include "bsml/shared/macros.hpp"
#include "HMUI/ViewController.hpp"

#include "bsml/shared/BSML/Components/Settings/IncrementSetting.hpp"
#include "GlobalNamespace/GameplaySetupViewController.hpp"

DECLARE_CLASS_CODEGEN(MultiplayerExtensions::UI, MpexEnvironmentViewController, HMUI::ViewController,
    DECLARE_INSTANCE_FIELD_PRIVATE(GlobalNamespace::GameplaySetupViewController*, _gameplaySetup);
    DECLARE_INSTANCE_FIELD_PRIVATE(BSML::IncrementSetting*, sideBySideDistanceIncrement);

    DECLARE_INJECT_METHOD(void, Inject, GlobalNamespace::GameplaySetupViewController* gameplaySetup);
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::il2cpp_type_check::MetadataGetter<&::HMUI::ViewController::DidActivate>::get(), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_INSTANCE_METHOD(void, PostParse);

    DECLARE_BSML_PROPERTY(bool, soloEnvironment);
    DECLARE_BSML_PROPERTY(bool, sideBySide);
    DECLARE_BSML_PROPERTY(float, sideBySideDistance);
    DECLARE_CTOR(ctor);
)
