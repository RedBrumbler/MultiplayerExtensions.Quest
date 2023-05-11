#include "Installers/MenuInstaller.hpp"


#include "Environment/MpexAvatarPlaceLighting.hpp"
#include "Patchers/AvatarPlacePatcher.hpp"
#include "Patchers/MenuEnvironmentPatcher.hpp"

#include "Zenject/DiContainer.hpp"
#include "Zenject/FromBinderNonGeneric.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"

#include "UI/MpexEnvironmentViewController.hpp"
#include "UI/MpexGameplaySetup.hpp"
#include "UI/MpexMiscViewController.hpp"
#include "UI/MpexSettingsViewController.hpp"
#include "UI/MpexSetupFlowCoordinator.hpp"

#include "lapiz/shared/utilities/ZenjectExtensions.hpp"
using namespace Lapiz::Zenject::ZenjectExtensions;

DEFINE_TYPE(MultiplayerExtensions::Installers, MenuInstaller);

namespace MultiplayerExtensions::Installers {
    void MenuInstaller::InstallBindings() {
        auto container = get_Container();

        container->BindInterfacesAndSelfTo<Patchers::AvatarPlacePatcher*>()->AsSingle();
        container->BindInterfacesAndSelfTo<Patchers::MenuEnvironmentPatcher*>()->AsSingle();

        FromNewComponentOnNewGameObject(container->BindInterfacesAndSelfTo<UI::MpexSetupFlowCoordinator*>())->AsSingle();
        FromNewComponentAsViewController(container->BindInterfacesAndSelfTo<UI::MpexSettingsViewController*>())->AsSingle();
        FromNewComponentAsViewController(container->BindInterfacesAndSelfTo<UI::MpexEnvironmentViewController*>())->AsSingle();
        FromNewComponentAsViewController(container->BindInterfacesAndSelfTo<UI::MpexMiscViewController*>())->AsSingle();
        container->BindInterfacesAndSelfTo<UI::MpexGameplaySetup*>()->AsSingle();

        // required for the local player place
        auto avatarPlace = container->Resolve<GlobalNamespace::MenuEnvironmentManager*>()->get_transform()->Find("MultiplayerLobbyEnvironment/LobbyAvatarPlace")->get_gameObject();
        UnityEngine::Object::Destroy(avatarPlace->GetComponent<Environment::MpexAvatarPlaceLighting*>());
        container->InstantiateComponent<Environment::MpexAvatarPlaceLighting*>(avatarPlace);
    }
}
