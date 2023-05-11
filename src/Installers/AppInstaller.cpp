#include "Installers/AppInstaller.hpp"

#include "Zenject/DiContainer.hpp"
#include "Zenject/FromBinderNonGeneric.hpp"

#include "Patchers/EnvironmentPatcher.hpp"
#include "Players/MpexPlayerManager.hpp"
#include "Utilities/SpriteManager.hpp"
#include "Utilities/SessionManagerEventPassthrough.hpp"

DEFINE_TYPE(MultiplayerExtensions::Installers, AppInstaller);

namespace MultiplayerExtensions::Installers {
    void AppInstaller::InstallBindings() {
        auto container = get_Container();

        container->BindInterfacesAndSelfTo<Utilities::SpriteManager*>()->AsSingle();
        container->BindInterfacesAndSelfTo<Utilities::SessionManagerEventPassthrough*>()->AsSingle();
        container->BindInterfacesAndSelfTo<Players::MpexPlayerManager*>()->AsSingle();
        container->BindInterfacesAndSelfTo<Patchers::EnvironmentPatcher*>()->AsSingle();
    }
}
