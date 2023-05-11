#include "hooking.hpp"
#include "logging.hpp"
#include "config.hpp"

#include "Patchers/MenuEnvironmentPatcher.hpp"

#include "GlobalNamespace/StandardLevelScenesTransitionSetupDataSO.hpp"
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

            auto multiScene = self->scenes.FirstOrDefault([](auto s){ return s->get_name()->Contains("Multiplayer"); });
            if (multiScene) {
                ListW<::GlobalNamespace::SceneInfo*> newScenes = List<::GlobalNamespace::SceneInfo*>::New_ctor();
                newScenes->EnsureCapacity(self->scenes.size() + 1);
                for (auto info : self->scenes) newScenes->Add(info);
                newScenes->Add(originalEnvironmentInfo->get_sceneInfo());
                self->scenes = newScenes->ToArray();
            } else {
                DEBUG("No multiplayer scene found");
            }

            // postfix, restore original info
            self->multiplayerEnvironmentInfo = originalEnvironmentInfo;
        } else {
            MultiplayerLevelScenesTransitionSetupDataSO_Init(self, gameMode, previewBeatmapLevel, beatmapDifficulty, beatmapCharacteristic, difficultyBeatmap, overrideColorScheme, gameplayModifiers, playerSpecificSettings, practiceSettings, useTestNoteCutSoundEffects);
        }
}

// we can't hook ScenesTransitionSetupDataSO_Init so we have to get creative with other hooks

MAKE_AUTO_HOOK_MATCH(StandardLevelScenesTransitionSetupDataSO_Init, &::GlobalNamespace::StandardLevelScenesTransitionSetupDataSO::Init, void, GlobalNamespace::StandardLevelScenesTransitionSetupDataSO* self, StringW gameMode, GlobalNamespace::IDifficultyBeatmap* difficultyBeatmap, GlobalNamespace::IPreviewBeatmapLevel* previewBeatmapLevel, GlobalNamespace::OverrideEnvironmentSettings* overrideEnvironmentSettings, GlobalNamespace::ColorScheme *overrideColorScheme, GlobalNamespace::GameplayModifiers *gameplayModifiers, GlobalNamespace::PlayerSpecificSettings *playerSpecificSettings, GlobalNamespace::PracticeSettings *practiceSettings, StringW backButtonText, bool useTestNoteCutSoundEffects, bool startPaused, GlobalNamespace::BeatmapDataCache* beatmapDataCache) {
    StandardLevelScenesTransitionSetupDataSO_Init(self, gameMode, difficultyBeatmap, previewBeatmapLevel, overrideEnvironmentSettings, overrideColorScheme, gameplayModifiers, playerSpecificSettings, practiceSettings, backButtonText, useTestNoteCutSoundEffects, startPaused, beatmapDataCache);
    auto& scenes = self->scenes;
    auto multiScene = self->scenes.FirstOrDefault([](auto s){ return s->get_name()->Contains("Multiplayer"); });
    if (multiScene) {
        ListW<::GlobalNamespace::SceneInfo*> newScenes = List<::GlobalNamespace::SceneInfo*>::New_ctor();
        newScenes->EnsureCapacity(scenes.size() + 1);
        for (auto info : scenes) newScenes->Add(info);
        newScenes->Add(originalEnvironmentInfo->get_sceneInfo());
        scenes = newScenes->ToArray();
    }
}
