#include "Patchers/MenuEnvironmentPatcher.hpp"

DEFINE_TYPE(MultiplayerExtensions::Patchers, MenuEnvironmentPatcher);

namespace MultiplayerExtensions::Patchers {
    MenuEnvironmentPatcher* MenuEnvironmentPatcher::instance;
    MenuEnvironmentPatcher* MenuEnvironmentPatcher::get_instance() { return instance; }

    void MenuEnvironmentPatcher::ctor(GlobalNamespace::GameplaySetupViewController* gameplaySetup) {
        INVOKE_CTOR();
        instance = this;
        _gameplaySetup = gameplaySetup;
    }

    void MenuEnvironmentPatcher::Dispose() {
        if (instance == this) instance = nullptr;
    }
}
