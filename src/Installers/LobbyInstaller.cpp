#include "Installers/LobbyInstaller.hpp"


#include "Environment/MpexAvatarPlaceLighting.hpp"
#include "Environment/MpexAvatarNameTag.hpp"

#include "Zenject/DiContainer.hpp"
#include "Zenject/ConcreteIdBinderGeneric_1.hpp"
#include "GlobalNamespace/MultiplayerLobbyInstaller.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"

#include "lapiz/shared/objects/RegistrationHelper.hpp"

DEFINE_TYPE(MultiplayerExtensions::Installers, LobbyInstaller);

namespace MultiplayerExtensions::Installers {
    using LobbyAvatarPlaceRegistration = Lapiz::Objects::Registration<GlobalNamespace::MultiplayerLobbyAvatarPlace*, GlobalNamespace::MultiplayerLobbyInstaller*>;
    using LobbyAvatarRegistration = Lapiz::Objects::Registration<GlobalNamespace::MultiplayerLobbyAvatarController*, GlobalNamespace::MultiplayerLobbyInstaller*>;

    void LobbyInstaller::InstallBindings() {
        auto container = get_Container();

        auto avatarPlaceRegistration = LobbyAvatarPlaceRegistration::New_ctor("multiplayerAvatarPlacePrefab", std::bind(&LobbyInstaller::DecorateAvatarPlace, this, std::placeholders::_1));
        auto avatarRegistration = LobbyAvatarRegistration::New_ctor("multiplayerLobbyAvatarControllerPrefab", std::bind(&LobbyInstaller::DecorateAvatar, this, std::placeholders::_1));

        avatarPlaceRegistration.RegisterRedecorator(container);
        avatarRegistration.RegisterRedecorator(container);
    }

    GlobalNamespace::MultiplayerLobbyAvatarPlace* LobbyInstaller::DecorateAvatarPlace(GlobalNamespace::MultiplayerLobbyAvatarPlace* original) {
        original->get_gameObject()->AddComponent<Environment::MpexAvatarPlaceLighting*>();
        return original;
    }

    GlobalNamespace::MultiplayerLobbyAvatarController* LobbyInstaller::DecorateAvatar(GlobalNamespace::MultiplayerLobbyAvatarController* original) {
        auto avatarCaption = original->get_transform()->Find("AvatarCaption")->get_gameObject();
        avatarCaption->AddComponent<Environment::MpexAvatarNameTag*>();

        return original;
    }
}
