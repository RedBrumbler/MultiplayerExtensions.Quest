#include "_config.h"
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

modloader::ModInfo modInfo{MOD_ID, VERSION, 0};

Logger& getLogger() {
    static Logger* logger = new Logger(modInfo, LoggerOptions(false, true));
    return *logger;
}

MPEX_EXPORT_FUNC void setup(CModInfo* info) {
    info->id = MOD_ID;
    info->version = VERSION;
    info->version_long = 0;
}

extern "C" void load() {

    if (!LoadConfig())
        SaveConfig();

    auto& logger = getLogger();
    il2cpp_functions::Init();
    custom_types::Register::AutoRegister();
    Hooks::InstallHooks(logger);
    BSML::Init();

    auto zenjector = Lapiz::Zenject::Zenjector::Get();
    zenjector->Install<MultiplayerExtensions::Installers::AppInstaller*>(Lapiz::Zenject::Location::App);
    zenjector->Install<MultiplayerExtensions::Installers::MenuInstaller*>(Lapiz::Zenject::Location::Menu);
    zenjector->Install<MultiplayerExtensions::Installers::LobbyInstaller*, GlobalNamespace::MultiplayerLobbyInstaller*>();
    zenjector->Install<MultiplayerExtensions::Installers::GameInstaller*>(Lapiz::Zenject::Location::MultiplayerCore);
    zenjector->Install<MultiplayerExtensions::Installers::LocalActivePlayerInstaller*>(Lapiz::Zenject::Location::Multi);
}
