#include "Patchers/MenuEnvironmentPatcher.hpp"

DEFINE_TYPE(MultiplayerExtensions::Patchers, MenuEnvironmentPatcher);

namespace MultiplayerExtensions::Patchers {
    MenuEnvironmentPatcher* MenuEnvironmentPatcher::instance;
    MenuEnvironmentPatcher* MenuEnvironmentPatcher::get_instance() { return instance; }

    void MenuEnvironmentPatcher::ctor(GlobalNamespace::GameplaySetupViewController* gameplaySetup, GlobalNamespace::EnvironmentsListModel* environmentsListModel) {
        INVOKE_CTOR();
        instance = this;
        _gameplaySetup = gameplaySetup;
        _environmentsListModel = environmentsListModel;
    }

    void MenuEnvironmentPatcher::Dispose() {
        if (instance == this) instance = nullptr;
    }
}
