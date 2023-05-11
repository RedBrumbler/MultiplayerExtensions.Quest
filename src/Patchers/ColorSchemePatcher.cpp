#include "Patchers/ColorSchemePatcher.hpp"

DEFINE_TYPE(MultiplayerExtensions::Patchers, ColorSchemePatcher);

namespace MultiplayerExtensions::Patchers {
    ColorSchemePatcher* ColorSchemePatcher::instance;
    ColorSchemePatcher* ColorSchemePatcher::get_instance() { return instance; }

    void ColorSchemePatcher::ctor(GlobalNamespace::GameplayCoreSceneSetupData* sceneSetupData) {
        INVOKE_CTOR();
        _sceneSetupData = sceneSetupData;
    }

    void ColorSchemePatcher::Dispose() {
        if (instance == this) instance = nullptr;
    }

    GlobalNamespace::ColorScheme* ColorSchemePatcher::get_colorScheme() {
        return _sceneSetupData->colorScheme;
    }
}
