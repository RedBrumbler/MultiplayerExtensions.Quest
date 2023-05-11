#include "hooking.hpp"
#include "logging.hpp"
#include "config.hpp"

#include "Patchers/ColorSchemePatcher.hpp"
#include "GlobalNamespace/PlayersSpecificSettingsAtGameStartModel.hpp"
#include "GlobalNamespace/PlayerSpecificSettingsNetSerializable.hpp"
#include "GlobalNamespace/ColorSchemeNetSerializable.hpp"
#include "GlobalNamespace/ColorScheme.hpp"

MAKE_AUTO_HOOK_MATCH(PlayersSpecificSettingsAtGameStartModel_GetPlayerSpecificSettingsForUserId, &::GlobalNamespace::PlayersSpecificSettingsAtGameStartModel::GetPlayerSpecificSettingsForUserId, GlobalNamespace::PlayerSpecificSettingsNetSerializable*, GlobalNamespace::PlayersSpecificSettingsAtGameStartModel* self, StringW userId) {
    auto patcher = MultiplayerExtensions::Patchers::ColorSchemePatcher::get_instance();
    auto res = PlayersSpecificSettingsAtGameStartModel_GetPlayerSpecificSettingsForUserId(self, userId);
    if (patcher) {
        auto colorscheme = patcher->get_colorScheme();
        if (colorscheme && config.disableMultiplayerColors) {
            res->colorScheme = GlobalNamespace::ColorSchemeNetSerializable(
                colorscheme->get_saberAColor(),
                colorscheme->get_saberBColor(),
                colorscheme->get_obstaclesColor(),
                colorscheme->get_environmentColor0(),
                colorscheme->get_environmentColor1(),
                colorscheme->get_environmentColor0Boost(),
                colorscheme->get_environmentColor1Boost()
            );
        }
    }
    return res;
}
