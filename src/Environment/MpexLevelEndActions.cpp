#include "Environment/MpexLevelEndActions.hpp"

DEFINE_TYPE(MultiplayerExtensions::Environment, MpexLevelEndActions);

namespace MultiplayerExtensions::Environment {
    MpexLevelEndActions* MpexLevelEndActions::instance;
    MpexLevelEndActions* MpexLevelEndActions::get_instance() { return instance; }

    void MpexLevelEndActions::ctor() {
        INVOKE_CTOR();
    }

    void MpexLevelEndActions::Dispose() {
        if (instance == this) instance = nullptr;
    }
}
