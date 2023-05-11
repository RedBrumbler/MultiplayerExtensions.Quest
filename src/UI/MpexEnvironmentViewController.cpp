#include "UI/MpexEnvironmentViewController.hpp"
#include "config.hpp"
#include "assets.hpp"

#include "bsml/shared/BSML.hpp"
#include "GlobalNamespace/PlayerSettingsPanelController_PlayerSettingsPanelLayout.hpp"

DEFINE_TYPE(MultiplayerExtensions::UI, MpexEnvironmentViewController);

namespace MultiplayerExtensions::UI {
    void MpexEnvironmentViewController::Inject(GlobalNamespace::GameplaySetupViewController* gameplaySetup) {
        _gameplaySetup = gameplaySetup;
    }

    void MpexEnvironmentViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
        if (!firstActivation) return;
        BSML::parse_and_construct(IncludedAssets::MpexEnvironmentViewController_bsml, get_transform(), this);
    }

    void MpexEnvironmentViewController::PostParse() {
        sideBySideDistanceIncrement->set_interactable(get_sideBySide());
    }

    bool MpexEnvironmentViewController::get_soloEnvironment() { return config.soloEnvironment; }
    void MpexEnvironmentViewController::set_soloEnvironment(bool value) {
        config.soloEnvironment = value;
        SaveConfig();
        _gameplaySetup->Setup(
            _gameplaySetup->showModifiers,
            _gameplaySetup->showEnvironmentOverrideSettings,
            _gameplaySetup->showColorSchemesSettings,
            _gameplaySetup->showMultiplayer,
            GlobalNamespace::PlayerSettingsPanelController::PlayerSettingsPanelLayout::Multiplayer
        );
    }

    bool MpexEnvironmentViewController::get_sideBySide() { return config.sideBySide; }
    void MpexEnvironmentViewController::set_sideBySide(bool value) {
        config.sideBySide = value;
        SaveConfig();
    }

    float MpexEnvironmentViewController::get_sideBySideDistance() { return config.sideBySideDistance; }
    void MpexEnvironmentViewController::set_sideBySideDistance(float value) {
        config.sideBySideDistance = value;
        SaveConfig();
    }
}
