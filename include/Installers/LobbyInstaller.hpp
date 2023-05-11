#pragma once

#include "custom-types/shared/macros.hpp"
#include "Zenject/Installer.hpp"

#include "GlobalNamespace/MultiplayerLobbyAvatarPlace.hpp"
#include "GlobalNamespace/MultiplayerLobbyAvatarController.hpp"

DECLARE_CLASS_CODEGEN(MultiplayerExtensions::Installers, LobbyInstaller, Zenject::Installer,
    DECLARE_OVERRIDE_METHOD(void, InstallBindings, il2cpp_utils::il2cpp_type_check::MetadataGetter<&::Zenject::Installer::InstallBindings>::get());

    DECLARE_INSTANCE_METHOD(GlobalNamespace::MultiplayerLobbyAvatarPlace*, DecorateAvatarPlace, GlobalNamespace::MultiplayerLobbyAvatarPlace* original);
    DECLARE_INSTANCE_METHOD(GlobalNamespace::MultiplayerLobbyAvatarController*, DecorateAvatar, GlobalNamespace::MultiplayerLobbyAvatarController* original);
    DECLARE_DEFAULT_CTOR();
)
