#pragma once

#include "custom-types/shared/macros.hpp"
#include "Zenject/Installer.hpp"

#include "GlobalNamespace/MultiplayerLocalActivePlayerFacade.hpp"
#include "GlobalNamespace/MultiplayerConnectedPlayerFacade.hpp"

DECLARE_CLASS_CODEGEN(MultiplayerExtensions::Installers, GameInstaller, Zenject::Installer,
    DECLARE_OVERRIDE_METHOD(void, InstallBindings, il2cpp_utils::il2cpp_type_check::MetadataGetter<&::Zenject::Installer::InstallBindings>::get());

    DECLARE_INSTANCE_METHOD(GlobalNamespace::MultiplayerLocalActivePlayerFacade*, DecorateLocalActivePlayerFacade, GlobalNamespace::MultiplayerLocalActivePlayerFacade* original);
    DECLARE_INSTANCE_METHOD(GlobalNamespace::MultiplayerConnectedPlayerFacade*, DecorateConnectedPlayerFacade, GlobalNamespace::MultiplayerConnectedPlayerFacade* original);
    DECLARE_DEFAULT_CTOR();
)
