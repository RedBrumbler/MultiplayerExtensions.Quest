#include "UI/MpexGameplaySetup.hpp"
#include "logging.hpp"
#include "config.hpp"
#include "assets.hpp"

#include "bsml/shared/BSML.hpp"

#include "UnityEngine/Transform.hpp"
#include "GlobalNamespace/PlayerSettingsPanelController.hpp"

DEFINE_TYPE(MultiplayerExtensions::UI, MpexGameplaySetup);

namespace MultiplayerExtensions::UI {
    void MpexGameplaySetup::Inject(GlobalNamespace::GameplaySetupViewController* gameplaySetup, GlobalNamespace::MainFlowCoordinator* mainFlowCoordinator, MpexSetupFlowCoordinator* setupFlowCoordinator) {
        _gameplaySetup = gameplaySetup;
        _multiplayerSettingsPanel = _gameplaySetup->_multiplayerSettingsPanelController;
        _mainFlowCoordinator = mainFlowCoordinator;
        _setupFlowCoordinator = setupFlowCoordinator;
    }

    void MpexGameplaySetup::Initialize() {
        BSML::parse_and_construct(Assets::Views::MpexGameplaySetup_bsml, _multiplayerSettingsPanel->get_transform(), this);
    }

    void MpexGameplaySetup::PresentPreferences() {
        auto flow = _mainFlowCoordinator->YoungestChildFlowCoordinatorOrSelf();
        _setupFlowCoordinator->_parentFlowCoordinator = flow;
        flow->PresentFlowCoordinator(_setupFlowCoordinator, nullptr, HMUI::ViewController::AnimationDirection::Horizontal, false, false);
    }

    bool MpexGameplaySetup::get_soloEnvironment() { return config.soloEnvironment; }

    void MpexGameplaySetup::set_soloEnvironment(bool value) {
        config.soloEnvironment = value;
        SaveConfig();
        _gameplaySetup->Setup(
            _gameplaySetup->_showModifiers,
            _gameplaySetup->_showEnvironmentOverrideSettings,
            _gameplaySetup->_showColorSchemesSettings,
            _gameplaySetup->_showMultiplayer,
            GlobalNamespace::PlayerSettingsPanelController::PlayerSettingsPanelLayout::Multiplayer
        );
    }
}
