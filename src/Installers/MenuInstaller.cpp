#include "Installers/MenuInstaller.hpp"


#include "Environment/MpexAvatarPlaceLighting.hpp"
#include "Patchers/AvatarPlacePatcher.hpp"
#include "Patchers/MenuEnvironmentPatcher.hpp"

#include "Zenject/DiContainer.hpp"
#include "Zenject/FromBinderNonGeneric.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"

DEFINE_TYPE(MultiplayerExtensions::Installers, MenuInstaller);

namespace MultiplayerExtensions::Installers {
    void MenuInstaller::InstallBindings() {
        auto container = get_Container();

        container->BindInterfacesAndSelfTo<Patchers::AvatarPlacePatcher*>()->AsSingle();
        container->BindInterfacesAndSelfTo<Patchers::MenuEnvironmentPatcher*>()->AsSingle();

        // TODO: a bunch of UI binds & installs

        // required for the local player place
        auto avatarPlace = container->Resolve<GlobalNamespace::MenuEnvironmentManager*>()->get_transform()->Find("MultiplayerLobbyEnvironment/LobbyAvatarPlace")->get_gameObject();
        UnityEngine::Object::Destroy(avatarPlace->GetComponent<Environment::MpexAvatarPlaceLighting*>());
        container->InstantiateComponent<Environment::MpexAvatarPlaceLighting*>(avatarPlace);
    }
}
