#include "UI/MpexSettingsViewController.hpp"
#include "config.hpp"
#include "assets.hpp"

#include "bsml/shared/BSML.hpp"

DEFINE_TYPE(MultiplayerExtensions::UI, MpexSettingsViewController);

namespace MultiplayerExtensions::UI {
    void MpexSettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
        if (!firstActivation) return;
        BSML::parse_and_construct(IncludedAssets::MpexSettingsViewController_bsml, get_transform(), this);
    }

    void MpexSettingsViewController::PostParse() {
        personalMissLightingToggle->set_interactable(get_missLighting());
    }

    bool MpexSettingsViewController::get_hidePlayerPlatforms() { return config.disableMultiplayerPlatforms; }
    void MpexSettingsViewController::set_hidePlayerPlatforms(bool value) {
        config.disableMultiplayerPlatforms = value;
        SaveConfig();
    }

    bool MpexSettingsViewController::get_hidePlayerLights() { return config.disableMultiplayerLights; }
    void MpexSettingsViewController::set_hidePlayerLights(bool value) {
        config.disableMultiplayerLights = value;
        SaveConfig();
    }

    bool MpexSettingsViewController::get_hidePlayerObjects() { return config.disableMultiplayerObjects; }
    void MpexSettingsViewController::set_hidePlayerObjects(bool value) {
        config.disableMultiplayerObjects = value;
        SaveConfig();
    }

    bool MpexSettingsViewController::get_missLighting() { return config.missLighting; }
    void MpexSettingsViewController::set_missLighting(bool value) {
        config.missLighting = value;
        if (personalMissLightingToggle)
            personalMissLightingToggle->set_interactable(value);
        SaveConfig();
    }

    bool MpexSettingsViewController::get_personalMissLightingOnly() { return config.personalMissLightingOnly; }
    void MpexSettingsViewController::set_personalMissLightingOnly(bool value) {
        config.personalMissLightingOnly = value;
        SaveConfig();
    }

}
