#include "UI/MpexMiscViewController.hpp"
#include "config.hpp"
#include "assets.hpp"

#include "bsml/shared/BSML.hpp"

DEFINE_TYPE(MultiplayerExtensions::UI, MpexMiscViewController);

namespace MultiplayerExtensions::UI {
    void MpexMiscViewController::ctor() {
        INVOKE_BASE_CTOR(classof(HMUI::ViewController*));
    }

    void MpexMiscViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
        if (!firstActivation) return;
        BSML::parse_and_construct(Assets::Views::MpexMiscViewController_bsml, get_transform(), this);
    }

    bool MpexMiscViewController::get_disablePlayerColors() { return config.disableMultiplayerColors; }
    void MpexMiscViewController::set_disablePlayerColors(bool value) {
        config.disableMultiplayerColors = value;
        SaveConfig();
    }

    bool MpexMiscViewController::get_disablePlatformMovement() { return config.disablePlatformMovement; }
    void MpexMiscViewController::set_disablePlatformMovement(bool value) {
        config.disablePlatformMovement = value;
        SaveConfig();
    }
}
