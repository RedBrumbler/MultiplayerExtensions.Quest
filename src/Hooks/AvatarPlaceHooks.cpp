#include "hooking.hpp"
#include "logging.hpp"

#include "Patchers/AvatarPlacePatcher.hpp"
#include "Environment/MpexAvatarPlaceLighting.hpp"

#include "GlobalNamespace/MultiplayerLobbyAvatarPlaceManager.hpp"
#include "GlobalNamespace/ILobbyStateDataModel.hpp"
#include "GlobalNamespace/IConnectedPlayer.hpp"

MAKE_AUTO_HOOK_MATCH(MultiplayerLobbyAvatarPlaceManager_SpawnAllPlaces, &::GlobalNamespace::MultiplayerLobbyAvatarPlaceManager::SpawnAllPlaces, void, GlobalNamespace::MultiplayerLobbyAvatarPlaceManager* self) {
    auto patcher = MultiplayerExtensions::Patchers::AvatarPlacePatcher::get_instance();
    int sortIndex = self->_lobbyStateDataModel->get_localPlayer()->get_sortIndex();
    if (patcher) patcher->SetSortIndex(sortIndex);

    MultiplayerLobbyAvatarPlaceManager_SpawnAllPlaces(self);
    for (auto place : ListW<GlobalNamespace::MultiplayerLobbyAvatarPlace*>(self->_allPlaces)) {
        auto lighting = place->GetComponent<MultiplayerExtensions::Environment::MpexAvatarPlaceLighting*>();
        if (lighting)
            lighting->_sortIndex = sortIndex;
        else ERROR("Can't get mpex avatar lighting from avatar place!");
    }
}
