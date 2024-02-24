#pragma once

#include "custom-types/shared/macros.hpp"
#include "bsml/shared/macros.hpp"
#include "HMUI/ViewController.hpp"

DECLARE_CLASS_CODEGEN(MultiplayerExtensions::UI, MpexMiscViewController, HMUI::ViewController,
    DECLARE_OVERRIDE_METHOD_MATCH(void, DidActivate, &::HMUI::ViewController::DidActivate, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);

    DECLARE_BSML_PROPERTY(bool, disablePlayerColors);
    DECLARE_BSML_PROPERTY(bool, disablePlatformMovement);
    DECLARE_CTOR(ctor);
)
