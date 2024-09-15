#pragma once

#include "custom-types/shared/macros.hpp"
#include "lapiz/shared/macros.hpp"
#include "bsml/shared/macros.hpp"
#include "bsml/shared/BSML/Components/Settings/ToggleSetting.hpp"

#include "HMUI/ViewController.hpp"

DECLARE_CLASS_CODEGEN(MultiplayerExtensions::UI, MpexSettingsViewController, HMUI::ViewController,
    DECLARE_INSTANCE_FIELD_PRIVATE(BSML::ToggleSetting*, personalMissLightingToggle);

    DECLARE_OVERRIDE_METHOD_MATCH(void, DidActivate, &::HMUI::ViewController::DidActivate, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_INSTANCE_METHOD(void, PostParse);

    DECLARE_BSML_PROPERTY(bool, hidePlayerPlatforms);
    DECLARE_BSML_PROPERTY(bool, hidePlayerLights);
    DECLARE_BSML_PROPERTY(bool, hidePlayerObjects);
    DECLARE_BSML_PROPERTY(bool, missLighting);
    DECLARE_BSML_PROPERTY(bool, personalMissLightingOnly);

    DECLARE_CTOR(ctor);
)
