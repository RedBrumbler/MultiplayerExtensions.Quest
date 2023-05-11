#include "hooking.hpp"
#include "logging.hpp"
#include "config.hpp"

#include "Patchers/MenuEnvironmentPatcher.hpp"

#include "GlobalNamespace/ScenesTransitionSetupDataSO.hpp"
#include "GlobalNamespace/MultiplayerLevelScenesTransitionSetupDataSO.hpp"
#include "GlobalNamespace/GameplaySetupViewController.hpp"
#include "GlobalNamespace/PlayerSettingsPanelController_PlayerSettingsPanelLayout.hpp"

#include "GlobalNamespace/SceneInfo.hpp"
#include "GlobalNamespace/IPreviewBeatmapLevel.hpp"
#include "GlobalNamespace/BeatmapDifficulty.hpp"
#include "GlobalNamespace/BeatmapCharacteristicSO.hpp"
#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/ColorScheme.hpp"
#include "GlobalNamespace/GameplayModifiers.hpp"
#include "GlobalNamespace/PlayerSpecificSettings.hpp"
#include "GlobalNamespace/PracticeSettings.hpp"
#include "GlobalNamespace/EnvironmentInfoSO.hpp"
#include "GlobalNamespace/BeatmapEnvironmentHelper.hpp"
#include "GlobalNamespace/OverrideEnvironmentSettings.hpp"

MAKE_AUTO_HOOK_MATCH(GameplaySetupViewController_Setup, &::GlobalNamespace::GameplaySetupViewController::Setup, void, GlobalNamespace::GameplaySetupViewController* self, bool showModifiers, bool showEnvironmentOverrideSettings, bool showColorSchemesSettings, bool showMultiplayer, ::GlobalNamespace::PlayerSettingsPanelController::PlayerSettingsPanelLayout playerSettingsPanelLayout) {
    GameplaySetupViewController_Setup(self, showModifiers, showEnvironmentOverrideSettings || (showMultiplayer && config.soloEnvironment), showColorSchemesSettings, showMultiplayer, playerSettingsPanelLayout);
}

static GlobalNamespace::EnvironmentInfoSO* originalEnvironmentInfo;

MAKE_AUTO_HOOK_MATCH(MultiplayerLevelScenesTransitionSetupDataSO_Init, &::GlobalNamespace::MultiplayerLevelScenesTransitionSetupDataSO::Init, void,
    GlobalNamespace::MultiplayerLevelScenesTransitionSetupDataSO* self,
    ::StringW gameMode,
    ::GlobalNamespace::IPreviewBeatmapLevel* previewBeatmapLevel,
    ::GlobalNamespace::BeatmapDifficulty beatmapDifficulty,
    ::GlobalNamespace::BeatmapCharacteristicSO* beatmapCharacteristic,
    ::GlobalNamespace::IDifficultyBeatmap* difficultyBeatmap,
    ::GlobalNamespace::ColorScheme* overrideColorScheme,
    ::GlobalNamespace::GameplayModifiers* gameplayModifiers,
    ::GlobalNamespace::PlayerSpecificSettings* playerSpecificSettings,
    ::GlobalNamespace::PracticeSettings* practiceSettings,
    bool useTestNoteCutSoundEffects) {
        auto patcher = MultiplayerExtensions::Patchers::MenuEnvironmentPatcher::get_instance();
        if (!patcher) {
            MultiplayerLevelScenesTransitionSetupDataSO_Init(self, gameMode, previewBeatmapLevel, beatmapDifficulty, beatmapCharacteristic, difficultyBeatmap, overrideColorScheme, gameplayModifiers, playerSpecificSettings, practiceSettings, useTestNoteCutSoundEffects);
            return;
        }

        if (config.soloEnvironment) {
            // prefix
            // save original info
            originalEnvironmentInfo = self->multiplayerEnvironmentInfo;
            self->multiplayerEnvironmentInfo = GlobalNamespace::BeatmapEnvironmentHelper::GetEnvironmentInfo(difficultyBeatmap);
            if (patcher->_gameplaySetup->get_environmentOverrideSettings()->overrideEnvironments)
                self->multiplayerEnvironmentInfo = patcher->_gameplaySetup->get_environmentOverrideSettings()->GetOverrideEnvironmentInfoForType(self->multiplayerEnvironmentInfo->get_environmentType());

            MultiplayerLevelScenesTransitionSetupDataSO_Init(self, gameMode, previewBeatmapLevel, beatmapDifficulty, beatmapCharacteristic, difficultyBeatmap, overrideColorScheme, gameplayModifiers, playerSpecificSettings, practiceSettings, useTestNoteCutSoundEffects);

            // postfix, restore original info
            self->multiplayerEnvironmentInfo = originalEnvironmentInfo;
        } else {
            MultiplayerLevelScenesTransitionSetupDataSO_Init(self, gameMode, previewBeatmapLevel, beatmapDifficulty, beatmapCharacteristic, difficultyBeatmap, overrideColorScheme, gameplayModifiers, playerSpecificSettings, practiceSettings, useTestNoteCutSoundEffects);
        }

}

MAKE_AUTO_HOOK_MATCH(ScenesTransitionSetupDataSO_Init, &::GlobalNamespace::ScenesTransitionSetupDataSO::Init, void, GlobalNamespace::ScenesTransitionSetupDataSO* self, ::ArrayW<::GlobalNamespace::SceneInfo*> scenes, ::ArrayW<::GlobalNamespace::SceneSetupData*> sceneSetupData) {
    auto multiScene = scenes.FirstOrDefault([](auto s){ return s->sceneName->Contains("Multiplayer"); });
    if (config.soloEnvironment && multiScene) {
        ListW<::GlobalNamespace::SceneInfo*> newScenes = List<::GlobalNamespace::SceneInfo*>::New_ctor();
        newScenes->EnsureCapacity(scenes.size() + 1);
        for (auto info : scenes) newScenes->Add(info);
        newScenes->Add(originalEnvironmentInfo->get_sceneInfo());
        scenes = newScenes->ToArray();
    }

    ScenesTransitionSetupDataSO_Init(self, scenes, sceneSetupData);
}
