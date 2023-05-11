#include "Patchers/AvatarPlacePatcher.hpp"

#include "Environment/MpexAvatarPlaceLighting.hpp"
#include "UnityEngine/Transform.hpp"

DEFINE_TYPE(MultiplayerExtensions::Patchers, AvatarPlacePatcher);

namespace MultiplayerExtensions::Patchers {
    AvatarPlacePatcher* AvatarPlacePatcher::instance;
    AvatarPlacePatcher* AvatarPlacePatcher::get_instance() { return instance; }

    void AvatarPlacePatcher::ctor(GlobalNamespace::MenuEnvironmentManager* menuEnvironmentManager) {
        INVOKE_CTOR();
        instance = this;
        _menuEnvironmentManager = menuEnvironmentManager;
    }

    void AvatarPlacePatcher::Dispose() {
        if (instance == this) instance = nullptr;
    }

    void AvatarPlacePatcher::SetSortIndex(int sortIndex) {
        _menuEnvironmentManager->get_transform()->Find("MultiplayerLobbyEnvironment/LobbyAvatarPlace")->GetComponent<Environment::MpexAvatarPlaceLighting*>()->_sortIndex = sortIndex;
    }
}
