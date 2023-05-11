#include "hooking.hpp"
#include "logging.hpp"

#include "GlobalNamespace/MultiplayerLocalActivePlayerFacade.hpp"
#include "Environment/MpexLevelEndActions.hpp"

MAKE_AUTO_HOOK_MATCH(MultiplayerLocalActivePlayerFacade_ReportPlayerDidFinish, &::GlobalNamespace::MultiplayerLocalActivePlayerFacade::ReportPlayerDidFinish, void, GlobalNamespace::MultiplayerLocalActivePlayerFacade* self, GlobalNamespace::MultiplayerLevelCompletionResults* results) {
    auto patcher = MultiplayerExtensions::Environment::MpexLevelEndActions::get_instance();
    if (patcher) patcher->LevelFinishedEvent.invoke();

    MultiplayerLocalActivePlayerFacade_ReportPlayerDidFinish(self, results);
}

MAKE_AUTO_HOOK_MATCH(MultiplayerLocalActivePlayerFacade_ReportPlayerNetworkDidFailed, &::GlobalNamespace::MultiplayerLocalActivePlayerFacade::ReportPlayerNetworkDidFailed, void, GlobalNamespace::MultiplayerLocalActivePlayerFacade* self, GlobalNamespace::MultiplayerLevelCompletionResults* results) {
    auto patcher = MultiplayerExtensions::Environment::MpexLevelEndActions::get_instance();
    if (patcher) patcher->LevelFailedEvent.invoke();

    MultiplayerLocalActivePlayerFacade_ReportPlayerNetworkDidFailed(self, results);
}
