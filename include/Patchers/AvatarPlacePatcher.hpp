#pragma once

#include "custom-types/shared/macros.hpp"

#include "GlobalNamespace/MenuEnvironmentManager.hpp"
#include "System/IDisposable.hpp"

DECLARE_CLASS_CODEGEN_INTERFACES(MultiplayerExtensions::Patchers, AvatarPlacePatcher, Il2CppObject, classof(System::IDisposable*),
    DECLARE_INSTANCE_FIELD_PRIVATE(GlobalNamespace::MenuEnvironmentManager*, _menuEnvironmentManager);
    DECLARE_OVERRIDE_METHOD(void, Dispose, il2cpp_utils::il2cpp_type_check::MetadataGetter<&::System::IDisposable::Dispose>::get());

    DECLARE_CTOR(ctor, GlobalNamespace::MenuEnvironmentManager* menuEnvironmentManager);

    public:
        static AvatarPlacePatcher* get_instance();
        void SetSortIndex(int sortIndex);
    private:
        static AvatarPlacePatcher* instance;
)
