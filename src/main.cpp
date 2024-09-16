#include "hooking.hpp"
#include "logging.hpp"
#include "config.hpp"

#include "custom-types/shared/register.hpp"

#include "Installers/AppInstaller.hpp"
#include "Installers/GameInstaller.hpp"
#include "Installers/LobbyInstaller.hpp"
#include "Installers/LocalActivePlayerInstaller.hpp"
#include "Installers/MenuInstaller.hpp"
#include "GlobalNamespace/MultiplayerLobbyInstaller.hpp"

#include "lapiz/shared/zenject/Zenjector.hpp"
#include "bsml/shared/BSML.hpp"

#include "_config.h"

modloader::ModInfo modInfo{MOD_ID, VERSION, VERSION_LONG};

MPEX_EXPORT_FUNC void setup(CModInfo* info) {
    info->id = MOD_ID;
    info->version = VERSION;
    info->version_long = VERSION_LONG;

    INFO("Mod '{}' with version '{}' version long '{}' finished setup", MOD_ID, VERSION, VERSION_LONG);
}

MPEX_EXPORT_FUNC void late_load() {

    if (!LoadConfig())
        SaveConfig();

    il2cpp_functions::Init();
    custom_types::Register::AutoRegister();
    MultiplayerExtensions::Hooking::InstallHooks();
    BSML::Init();

    auto zenjector = Lapiz::Zenject::Zenjector::Get();
    zenjector->Install<MultiplayerExtensions::Installers::AppInstaller*>(Lapiz::Zenject::Location::App);
    zenjector->Install<MultiplayerExtensions::Installers::MenuInstaller*>(Lapiz::Zenject::Location::Menu);
    zenjector->Install<MultiplayerExtensions::Installers::LobbyInstaller*, GlobalNamespace::MultiplayerLobbyInstaller*>();
    zenjector->Install<MultiplayerExtensions::Installers::GameInstaller*>(Lapiz::Zenject::Location::MultiplayerCore);
    zenjector->Install<MultiplayerExtensions::Installers::LocalActivePlayerInstaller*>(Lapiz::Zenject::Location::Multi);

    INFO("Mod '{}' with version '{}' version long {} finished late load", MOD_ID, VERSION, VERSION_LONG);
}
