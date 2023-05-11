#pragma once

#include "custom-types/shared/macros.hpp"

#include "GlobalNamespace/GameplayCoreSceneSetupData.hpp"
#include "System/IDisposable.hpp"

DECLARE_CLASS_CODEGEN_INTERFACES(MultiplayerExtensions::Patchers, ColorSchemePatcher, Il2CppObject, classof(System::IDisposable*),
    DECLARE_INSTANCE_FIELD_PRIVATE(GlobalNamespace::GameplayCoreSceneSetupData*, _sceneSetupData);
    DECLARE_OVERRIDE_METHOD(void, Dispose, il2cpp_utils::il2cpp_type_check::MetadataGetter<&::System::IDisposable::Dispose>::get());

    DECLARE_CTOR(ctor, GlobalNamespace::GameplayCoreSceneSetupData* sceneSetupData);

    public:
        static ColorSchemePatcher* get_instance();
        GlobalNamespace::ColorScheme* get_colorScheme();
    private:
        static ColorSchemePatcher* instance;
)
