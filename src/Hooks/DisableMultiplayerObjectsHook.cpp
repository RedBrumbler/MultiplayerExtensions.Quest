#include "hooking.hpp"
#include "logging.hpp"
#include "config.hpp"

#include "GlobalNamespace/MultiplayerConnectedPlayerFacade.hpp"

// possibly does not run orig
MAKE_AUTO_HOOK_ORIG_MATCH(MultiplayerConnectedPlayerFacade_ResumeSpawning, &::GlobalNamespace::MultiplayerConnectedPlayerFacade::ResumeSpawning, void, GlobalNamespace::MultiplayerConnectedPlayerFacade* self) {
    if (config.disableMultiplayerObjects) return;
    MultiplayerConnectedPlayerFacade_ResumeSpawning(self);
}
