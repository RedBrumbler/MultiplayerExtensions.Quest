#include "hooking.hpp"
#include "logging.hpp"
#include "config.hpp"

#include "GlobalNamespace/MultiplayerVerticalPlayerMovementManager.hpp"

// possibly does not run orig
MAKE_AUTO_HOOK_ORIG_MATCH(MultiplayerVerticalPlayerMovementManager_Update, &::GlobalNamespace::MultiplayerVerticalPlayerMovementManager::Update, void, GlobalNamespace::MultiplayerVerticalPlayerMovementManager* self) {
    if (config.disablePlatformMovement) return;
    MultiplayerVerticalPlayerMovementManager_Update(self);
}
