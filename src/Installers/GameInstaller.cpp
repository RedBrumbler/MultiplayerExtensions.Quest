#include "Installers/GameInstaller.hpp"
#include "config.hpp"

#include "Environment/MpexPlayerFacadeLighting.hpp"
#include "Environment/MpexConnectedObjectManager.hpp"
#include "Patchers/ColorSchemePatcher.hpp"

#include "Zenject/DiContainer.hpp"
#include "Zenject/FromBinderNonGeneric.hpp"
#include "Zenject/ConcreteIdBinderGeneric_1.hpp"
#include "UnityEngine/GameObject.hpp"
#include "GlobalNamespace/MultiplayerPlayersManager.hpp"

#include "lapiz/shared/objects/RegistrationHelper.hpp"

DEFINE_TYPE(MultiplayerExtensions::Installers, GameInstaller);

namespace MultiplayerExtensions::Installers {
    using LocalActivePlayerRegistration = Lapiz::Objects::Registration<GlobalNamespace::MultiplayerLocalActivePlayerFacade*, GlobalNamespace::MultiplayerPlayersManager*>;
    using ConnectedPlayerRegistration = Lapiz::Objects::Registration<GlobalNamespace::MultiplayerConnectedPlayerFacade*, GlobalNamespace::MultiplayerPlayersManager*>;

    void GameInstaller::InstallBindings() {
        auto container = get_Container();

        container->BindInterfacesAndSelfTo<Patchers::ColorSchemePatcher*>()->AsSingle();

        auto localPlayerRegistration = LocalActivePlayerRegistration::New_ctor("_activeLocalPlayerControllerPrefab", std::bind(&GameInstaller::DecorateLocalActivePlayerFacade, this, std::placeholders::_1));
        auto localDuelPlayerRegistration = LocalActivePlayerRegistration::New_ctor("_activeLocalPlayerDuelControllerPrefab", std::bind(&GameInstaller::DecorateLocalActivePlayerFacade, this, std::placeholders::_1));
        auto connectedPlayerRegistration = ConnectedPlayerRegistration::New_ctor("_connectedPlayerControllerPrefab", std::bind(&GameInstaller::DecorateConnectedPlayerFacade, this, std::placeholders::_1));
        auto connectedDuelPlayerRegistration = ConnectedPlayerRegistration::New_ctor("_connectedPlayerDuelControllerPrefab", std::bind(&GameInstaller::DecorateConnectedPlayerFacade, this, std::placeholders::_1));

        localPlayerRegistration.RegisterRedecorator(container);
        localDuelPlayerRegistration.RegisterRedecorator(container);
        connectedPlayerRegistration.RegisterRedecorator(container);
        connectedDuelPlayerRegistration.RegisterRedecorator(container);
    }

    GlobalNamespace::MultiplayerLocalActivePlayerFacade* GameInstaller::DecorateLocalActivePlayerFacade(GlobalNamespace::MultiplayerLocalActivePlayerFacade* original) {
        if (config.missLighting)
            original->get_gameObject()->AddComponent<Environment::MpexPlayerFacadeLighting*>();
        return original;
    }

    GlobalNamespace::MultiplayerConnectedPlayerFacade* GameInstaller::DecorateConnectedPlayerFacade(GlobalNamespace::MultiplayerConnectedPlayerFacade* original) {
        if (config.missLighting && !config.personalMissLightingOnly)
            original->get_gameObject()->AddComponent<Environment::MpexPlayerFacadeLighting*>();
        original->get_gameObject()->AddComponent<Environment::MpexConnectedObjectManager*>();
        return original;
    }


}
