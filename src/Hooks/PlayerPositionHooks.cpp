#include "hooking.hpp"
#include "logging.hpp"
#include "config.hpp"

#include "UnityEngine/Vector3.hpp"
#include "GlobalNamespace/MultiplayerPlayerLayout.hpp"
#include "GlobalNamespace/MultiplayerPlayerPlacement.hpp"
#include "GlobalNamespace/MultiplayerConditionalActiveByLayout.hpp"
#include "GlobalNamespace/MultiplayerLayoutProvider.hpp"

MAKE_AUTO_HOOK_MATCH(MultiplayerLayoutProvider_CalculateLayout, &::GlobalNamespace::MultiplayerLayoutProvider::CalculateLayout, GlobalNamespace::MultiplayerPlayerLayout, GlobalNamespace::MultiplayerLayoutProvider* self, int activePlayerCount) {
    if (config.sideBySide) return GlobalNamespace::MultiplayerPlayerLayout::Duel;
    return MultiplayerLayoutProvider_CalculateLayout(self, activePlayerCount);
}

MAKE_AUTO_HOOK_MATCH(MultiplayerConditionalActiveByLayout_Start, &::GlobalNamespace::MultiplayerConditionalActiveByLayout::Start, void, GlobalNamespace::MultiplayerConditionalActiveByLayout* self) {
    if (config.sideBySide && self->_layoutProvider->get_layout() == GlobalNamespace::MultiplayerPlayerLayout::NotDetermined)
        self->HandlePlayersLayoutWasCalculated(GlobalNamespace::MultiplayerPlayerLayout::Duel, 2);

    MultiplayerConditionalActiveByLayout_Start(self);
}

MAKE_AUTO_HOOK_MATCH(MultiplayerConditionalActiveByLayout_HandlePlayersLayoutWasCalculated, &::GlobalNamespace::MultiplayerConditionalActiveByLayout::HandlePlayersLayoutWasCalculated, void, GlobalNamespace::MultiplayerConditionalActiveByLayout* self, GlobalNamespace::MultiplayerPlayerLayout layout, int playersCount) {
    if (config.sideBySide)
        layout = GlobalNamespace::MultiplayerPlayerLayout::Duel;

    MultiplayerConditionalActiveByLayout_HandlePlayersLayoutWasCalculated(self, layout, playersCount);
}

MAKE_AUTO_HOOK_MATCH(MultiplayerPlayerPlacement_HandlePlayersLayoutWasCalculated, &::GlobalNamespace::MultiplayerPlayerPlacement::GetPlayerWorldPosition, UnityEngine::Vector3, float outerCircleRadius, float outerCirclePositionAngle, ::GlobalNamespace::MultiplayerPlayerLayout layout) {
    auto res = MultiplayerPlayerPlacement_HandlePlayersLayoutWasCalculated(outerCircleRadius, outerCirclePositionAngle, layout);
    if (config.sideBySide) {
        auto sortIndex = outerCirclePositionAngle;
        res = {sortIndex * 100.0f * config.sideBySideDistance, 0, 0};
    }
    return res;
}
