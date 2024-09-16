#include "hooking.hpp"
#include "logging.hpp"
#include "config.hpp"

#include "Patchers/MenuEnvironmentPatcher.hpp"

#include "GlobalNamespace/ScenesTransitionSetupDataSO.hpp"
#include "GlobalNamespace/MultiplayerLevelScenesTransitionSetupDataSO.hpp"
#include "GlobalNamespace/GameplaySetupViewController.hpp"

#include "GlobalNamespace/BeatmapLevel.hpp"
#include "GlobalNamespace/SceneInfo.hpp"
#include "GlobalNamespace/ColorScheme.hpp"
#include "GlobalNamespace/GameplayModifiers.hpp"
#include "GlobalNamespace/PlayerSpecificSettings.hpp"
#include "GlobalNamespace/PracticeSettings.hpp"
#include "GlobalNamespace/EnvironmentInfoSO.hpp"
#include "GlobalNamespace/OverrideEnvironmentSettings.hpp"

MAKE_AUTO_HOOK_MATCH(GameplaySetupViewController_Setup, &::GlobalNamespace::GameplaySetupViewController::Setup, void, GlobalNamespace::GameplaySetupViewController* self, bool showModifiers, bool showEnvironmentOverrideSettings, bool showColorSchemesSettings, bool showMultiplayer, ::GlobalNamespace::PlayerSettingsPanelController::PlayerSettingsPanelLayout playerSettingsPanelLayout) {
    GameplaySetupViewController_Setup(self, showModifiers, showEnvironmentOverrideSettings || (showMultiplayer && config.soloEnvironment), showColorSchemesSettings, showMultiplayer, playerSettingsPanelLayout);
}

static GlobalNamespace::EnvironmentInfoSO* originalEnvironmentInfo;

MAKE_AUTO_HOOK_MATCH(MultiplayerLevelScenesTransitionSetupDataSO_Init, &::GlobalNamespace::MultiplayerLevelScenesTransitionSetupDataSO::Init, void,
    GlobalNamespace::MultiplayerLevelScenesTransitionSetupDataSO* self,
    ::StringW gameMode,
    ByRef<::GlobalNamespace::BeatmapKey> beatmapKey,
    ::GlobalNamespace::BeatmapLevel* beatmapLevel,
    ::GlobalNamespace::IBeatmapLevelData* beatmapLevelData,
    ::GlobalNamespace::ColorScheme* overrideColorScheme,
    ::GlobalNamespace::GameplayModifiers* gameplayModifiers,
    ::GlobalNamespace::PlayerSpecificSettings* playerSpecificSettings,
    ::GlobalNamespace::PracticeSettings* practiceSettings,
    ::GlobalNamespace::AudioClipAsyncLoader* audioClipAsyncLoader,
    ::BeatSaber::PerformancePresets::PerformancePreset* performancePreset,
    ::GlobalNamespace::BeatmapDataLoader * beatmapDataLoader,
    bool useTestNoteCutSoundEffects) {
        auto patcher = MultiplayerExtensions::Patchers::MenuEnvironmentPatcher::get_instance();
        if (!patcher) {
            ERROR("MenuEnvironmentPatcher instance is null");
            MultiplayerLevelScenesTransitionSetupDataSO_Init(self, gameMode, beatmapKey, beatmapLevel, beatmapLevelData, overrideColorScheme, gameplayModifiers, playerSpecificSettings, practiceSettings, audioClipAsyncLoader, performancePreset, beatmapDataLoader, useTestNoteCutSoundEffects);
            return;
        }

        if (config.soloEnvironment) {
            // prefix
            // save original info
            StringW environmentName = beatmapLevel->GetEnvironmentName(beatmapKey->beatmapCharacteristic, beatmapKey->difficulty)._environmentName;
            GlobalNamespace::EnvironmentInfoSO* environmentInfo = patcher->_environmentsListModel->GetEnvironmentInfoBySerializedNameSafe(environmentName);
            originalEnvironmentInfo = self->GetOrLoadMultiplayerEnvironmentInfo();
            self->_loadedMultiplayerEnvironmentInfo = environmentInfo;
            if (patcher->_gameplaySetup->get_environmentOverrideSettings()->overrideEnvironments)
                self->_loadedMultiplayerEnvironmentInfo = patcher->_gameplaySetup->get_environmentOverrideSettings()->GetOverrideEnvironmentInfoForType(self->_loadedMultiplayerEnvironmentInfo->get_environmentType());
            DEBUG("Replacing Original environment info: {} with solo environment info: {}", originalEnvironmentInfo->name, self->_loadedMultiplayerEnvironmentInfo->name);

            MultiplayerLevelScenesTransitionSetupDataSO_Init(self, gameMode, beatmapKey, beatmapLevel, beatmapLevelData, overrideColorScheme, gameplayModifiers, playerSpecificSettings, practiceSettings, audioClipAsyncLoader, performancePreset, beatmapDataLoader, useTestNoteCutSoundEffects);

            // auto multiScene = self->scenes->FirstOrDefault([](auto s){ return s->get_name()->Contains("Multiplayer"); });
            // if (multiScene) {
            //     ListW<::GlobalNamespace::SceneInfo*> newScenes = ListW<::GlobalNamespace::SceneInfo*>::New();
            //     newScenes->EnsureCapacity(self->scenes.size() + 1);
		    //     // Ensures the original environment info comes before GameCore as some mods rely on GameCore being present on the scene switch callback
            //     for (auto info : self->scenes) {
            //         if (info->name == "GameCore") newScenes->Add(originalEnvironmentInfo->get_sceneInfo());
            //         newScenes->Add(info);
            //     }
            //     self->scenes = newScenes->ToArray();
            // } else {
            //     DEBUG("No multiplayer scene found");
            // }

            // postfix, restore original info
            self->_loadedMultiplayerEnvironmentInfo = originalEnvironmentInfo;
        } else {
            MultiplayerLevelScenesTransitionSetupDataSO_Init(self, gameMode, beatmapKey, beatmapLevel, beatmapLevelData, overrideColorScheme, gameplayModifiers, playerSpecificSettings, practiceSettings, audioClipAsyncLoader, performancePreset, beatmapDataLoader, useTestNoteCutSoundEffects);
        }
}

// we can't hook ScenesTransitionSetupDataSO_Init so we have to get creative with other hooks

MAKE_AUTO_HOOK_MATCH(MultiplayerLevelScenesTransitionSetupDataSO_InitAndSetupScenes, &::GlobalNamespace::MultiplayerLevelScenesTransitionSetupDataSO::InitAndSetupScenes, void, ::GlobalNamespace::MultiplayerLevelScenesTransitionSetupDataSO* self)
{
    MultiplayerLevelScenesTransitionSetupDataSO_InitAndSetupScenes(self);
    // TODO: Possibly check sceneName here instead of name
    if (config.soloEnvironment && self->scenes->FirstOrDefault([](auto s){ return s->get_name()->Contains("Multiplayer"); })) {
        DEBUG("At least one scenes name contains Multiplayer, adding original env info");
        ListW<::GlobalNamespace::SceneInfo*> newScenes = ListW<::GlobalNamespace::SceneInfo*>::New();
        newScenes->EnsureCapacity(self->scenes.size() + 1);
        for (auto info : self->scenes) {
            // Ensures the original environment info comes before GameCore as some mods rely on GameCore being present on the scene switch callback
            if (info->sceneName == "GameCore") {
                DEBUG("Adding original environment info before GameCore");
                newScenes->Add(originalEnvironmentInfo->get_sceneInfo());
            }
            DEBUG("Adding scene {}", info->sceneName);
            newScenes->Add(info);
        }
        self->scenes = newScenes->ToArray();
    }
}
