#include "Installers/LocalActivePlayerInstaller.hpp"

#include "Zenject/DiContainer.hpp"
#include "Zenject/FromBinderNonGeneric.hpp"
#include "Zenject/ConcreteIdBinderGeneric_1.hpp"

#include "Environment/MpexLevelEndActions.hpp"
#include "GlobalNamespace/EnvironmentContext.hpp"

DEFINE_TYPE(MultiplayerExtensions::Installers, LocalActivePlayerInstaller);

namespace MultiplayerExtensions::Installers {
    void LocalActivePlayerInstaller::InstallBindings() {
        auto container = get_Container();

        container->BindInterfacesAndSelfTo<Environment::MpexLevelEndActions*>()->AsSingle();
        container->Bind<GlobalNamespace::EnvironmentContext>()->FromInstance(GlobalNamespace::EnvironmentContext::Gameplay)->AsSingle();
    }
}
