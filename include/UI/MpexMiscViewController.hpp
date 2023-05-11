#pragma once

#include "custom-types/shared/macros.hpp"
#include "bsml/shared/macros.hpp"
#include "HMUI/ViewController.hpp"

DECLARE_CLASS_CODEGEN(MultiplayerExtensions::UI, MpexMiscViewController, HMUI::ViewController,
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::il2cpp_type_check::MetadataGetter<&::HMUI::ViewController::DidActivate>::get(), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);

    DECLARE_BSML_PROPERTY(bool, disablePlayerColors);
    DECLARE_BSML_PROPERTY(bool, disablePlatformMovement);
    DECLARE_CTOR(ctor);
)
