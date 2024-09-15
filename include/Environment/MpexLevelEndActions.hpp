#pragma once

#include "custom-types/shared/macros.hpp"

#include "GlobalNamespace/MenuEnvironmentManager.hpp"
#include "System/IDisposable.hpp"

DECLARE_CLASS_CODEGEN_INTERFACES(MultiplayerExtensions::Environment, MpexLevelEndActions, Il2CppObject, classof(System::IDisposable*),
    DECLARE_OVERRIDE_METHOD_MATCH(void, Dispose, &::System::IDisposable::Dispose);

    DECLARE_CTOR(ctor);
    public:
        static MpexLevelEndActions* get_instance();

        UnorderedEventCallback<> LevelFailedEvent;
        UnorderedEventCallback<> LevelFinishedEvent;
    private:
        static MpexLevelEndActions* instance;
)
