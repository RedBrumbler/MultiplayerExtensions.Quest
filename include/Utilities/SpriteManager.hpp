#pragma once

#include "custom-types/shared/macros.hpp"
#include "UnityEngine/Sprite.hpp"

#include "Zenject/IInitializable.hpp"
#include "System/IDisposable.hpp"

DECLARE_CLASS_CODEGEN_INTERFACES(MultiplayerExtensions::Utilities, SpriteManager, Il2CppObject, std::vector<Il2CppClass*>({classof(Zenject::IInitializable*), classof(System::IDisposable*)}),
    DECLARE_INSTANCE_FIELD_PRIVATE(UnityEngine::Sprite*, _oculusIcon);
    DECLARE_INSTANCE_FIELD_PRIVATE(UnityEngine::Sprite*, _steamIcon);
    DECLARE_INSTANCE_FIELD_PRIVATE(UnityEngine::Sprite*, _metaIcon);
    DECLARE_INSTANCE_FIELD_PRIVATE(UnityEngine::Sprite*, _toasterIcon);

    DECLARE_OVERRIDE_METHOD(void, Initialize, il2cpp_utils::il2cpp_type_check::MetadataGetter<&::Zenject::IInitializable::Initialize>::get());
    DECLARE_OVERRIDE_METHOD(void, Dispose, il2cpp_utils::il2cpp_type_check::MetadataGetter<&::System::IDisposable::Dispose>::get());

    DECLARE_DEFAULT_CTOR();

    public:
        UnityEngine::Sprite* get_oculusIcon() const { return _oculusIcon; }
        UnityEngine::Sprite* get_steamIcon() const { return _steamIcon; }
        UnityEngine::Sprite* get_metaIcon() const { return _metaIcon; }
        UnityEngine::Sprite* get_toasterIcon() const { return _toasterIcon; }
)
