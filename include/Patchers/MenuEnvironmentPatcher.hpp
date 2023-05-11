#pragma once

#include "custom-types/shared/macros.hpp"

#include "GlobalNamespace/GameplaySetupViewController.hpp"
#include "System/IDisposable.hpp"

struct Hook_MultiplayerLevelScenesTransitionSetupDataSO_Init;

DECLARE_CLASS_CODEGEN_INTERFACES(MultiplayerExtensions::Patchers, MenuEnvironmentPatcher, Il2CppObject, classof(System::IDisposable*),
    DECLARE_INSTANCE_FIELD_PRIVATE(GlobalNamespace::GameplaySetupViewController*, _gameplaySetup);
    DECLARE_OVERRIDE_METHOD(void, Dispose, il2cpp_utils::il2cpp_type_check::MetadataGetter<&::System::IDisposable::Dispose>::get());

    DECLARE_CTOR(ctor, GlobalNamespace::GameplaySetupViewController* gameplaySetup);

    public:
        static MenuEnvironmentPatcher* get_instance();
    private:
        friend ::Hook_MultiplayerLevelScenesTransitionSetupDataSO_Init;
        static MenuEnvironmentPatcher* instance;
)
