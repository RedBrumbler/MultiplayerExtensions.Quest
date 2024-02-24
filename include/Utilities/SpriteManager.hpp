#pragma once

#include "custom-types/shared/macros.hpp"
#include "UnityEngine/Sprite.hpp"

#include "Zenject/IInitializable.hpp"
#include "System/IDisposable.hpp"

DECLARE_CLASS_CODEGEN_INTERFACES(MultiplayerExtensions::Utilities, SpriteManager, System::Object, std::vector<Il2CppClass*>({classof(Zenject::IInitializable*), classof(System::IDisposable*)}),
    DECLARE_INSTANCE_FIELD_PRIVATE(UnityEngine::Sprite*, _oculusIcon);
    DECLARE_INSTANCE_FIELD_PRIVATE(UnityEngine::Sprite*, _steamIcon);
    DECLARE_INSTANCE_FIELD_PRIVATE(UnityEngine::Sprite*, _metaIcon);
    DECLARE_INSTANCE_FIELD_PRIVATE(UnityEngine::Sprite*, _toasterIcon);

    DECLARE_OVERRIDE_METHOD_MATCH(void, Initialize, &::Zenject::IInitializable::Initialize);
    DECLARE_OVERRIDE_METHOD_MATCH(void, Dispose, &::System::IDisposable::Dispose);

    DECLARE_DEFAULT_CTOR();

    public:
        __declspec(property(get=get_oculusIcon)) UnityEngine::Sprite* OculusIcon;
        __declspec(property(get=get_steamIcon)) UnityEngine::Sprite* SteamIcon;
        __declspec(property(get=get_metaIcon)) UnityEngine::Sprite* MetaIcon;
        __declspec(property(get=get_toasterIcon)) UnityEngine::Sprite* ToasterIcon;

        UnityEngine::Sprite* get_oculusIcon() const { return _oculusIcon; }
        UnityEngine::Sprite* get_steamIcon() const { return _steamIcon; }
        UnityEngine::Sprite* get_metaIcon() const { return _metaIcon; }
        UnityEngine::Sprite* get_toasterIcon() const { return _toasterIcon; }
)
