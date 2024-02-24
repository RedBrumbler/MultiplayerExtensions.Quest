#include "UI/MpexSetupFlowCoordinator.hpp"

#include "custom-types/shared/delegate.hpp"

DEFINE_TYPE(MultiplayerExtensions::UI, MpexSetupFlowCoordinator);

namespace MultiplayerExtensions::UI {
    void MpexSetupFlowCoordinator::ctor() {
        INVOKE_BASE_CTOR(classof(HMUI::FlowCoordinator*));
        _dismissGameStartedAction = custom_types::MakeDelegate<System::Action_1<GlobalNamespace::ILevelGameplaySetupData*>*>(
            std::function<void(GlobalNamespace::ILevelGameplaySetupData*)>(
                std::bind(&MpexSetupFlowCoordinator::DismissGameStartedEvent, this, std::placeholders::_1)
            )
        );
    }

    void MpexSetupFlowCoordinator::Inject(GlobalNamespace::MainFlowCoordinator* mainFlowCoordinator, MpexSettingsViewController* settingsViewController, MpexEnvironmentViewController* environmentViewController, MpexMiscViewController* miscViewController, GlobalNamespace::ILobbyGameStateController* gameStateController) {
        _mainFlowCoordinator = mainFlowCoordinator;
        _settingsViewController = settingsViewController;
        _environmentViewController = environmentViewController;
        _miscViewController = miscViewController;
        _gameStateController = gameStateController;
    }

    void MpexSetupFlowCoordinator::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
        if (firstActivation) {
            SetTitle("Multiplayer Preferences", HMUI::ViewController::AnimationType::In);
            set_showBackButton(true);
        }

        if (addedToHierarchy) {
            ProvideInitialViewControllers(_settingsViewController, _environmentViewController, _miscViewController, nullptr, nullptr);
            _gameStateController->add_gameStartedEvent(_dismissGameStartedAction);
        }
    }

    void MpexSetupFlowCoordinator::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling) {
        if (removedFromHierarchy)
            _gameStateController->remove_gameStartedEvent(_dismissGameStartedAction);
    }

    void MpexSetupFlowCoordinator::BackButtonWasPressed(HMUI::ViewController* topViewController) {
        _parentFlowCoordinator->DismissFlowCoordinator(this, HMUI::ViewController::AnimationDirection::Horizontal, nullptr, false);
    }

    void MpexSetupFlowCoordinator::DismissGameStartedEvent(GlobalNamespace::ILevelGameplaySetupData* _) {
        _parentFlowCoordinator->DismissFlowCoordinator(this, HMUI::ViewController::AnimationDirection::Horizontal, nullptr, true);
    }
}
