#pragma once

#include "custom-types/shared/macros.hpp"

#include "GlobalNamespace/GameScenesManager.hpp"
#include "GlobalNamespace/EnvironmentSceneSetup.hpp"
#include "GlobalNamespace/GameplayCoreInstaller.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/GameObject.hpp"
#include "Zenject/InstallerBase.hpp"
#include "Zenject/ScriptableObjectInstaller.hpp"
#include "Zenject/SceneDecoratorContext.hpp"
#include "Zenject/GameObjectContext.hpp"
#include "Zenject/Context.hpp"
#include "Zenject/DiContainer.hpp"
#include "Zenject/MonoInstaller.hpp"
#include "System/Type.hpp"

#include "System/IDisposable.hpp"

DECLARE_CLASS_CODEGEN_INTERFACES(MultiplayerExtensions::Patchers, EnvironmentPatcher, Il2CppObject, classof(System::IDisposable*),
    DECLARE_INSTANCE_FIELD_PRIVATE(GlobalNamespace::GameScenesManager*, _scenesManager);
    DECLARE_OVERRIDE_METHOD(void, Dispose, il2cpp_utils::il2cpp_type_check::MetadataGetter<&::System::IDisposable::Dispose>::get());

    DECLARE_INSTANCE_FIELD_PRIVATE_DEFAULT(ListW<UnityEngine::MonoBehaviour*>, _behavioursToInject, List<UnityEngine::MonoBehaviour*>::New_ctor());
    DECLARE_INSTANCE_FIELD_PRIVATE_DEFAULT(ListW<Zenject::InstallerBase*>, _normalInstallers, List<Zenject::InstallerBase*>::New_ctor());
    DECLARE_INSTANCE_FIELD_PRIVATE_DEFAULT(ListW<System::Type*>, _normalInstallerTypes, List<System::Type*>::New_ctor());
    DECLARE_INSTANCE_FIELD_PRIVATE_DEFAULT(ListW<Zenject::ScriptableObjectInstaller*>, _scriptableObjectInstallers, List<Zenject::ScriptableObjectInstaller*>::New_ctor());
    DECLARE_INSTANCE_FIELD_PRIVATE_DEFAULT(ListW<Zenject::MonoInstaller*>, _monoInstallers, List<Zenject::MonoInstaller*>::New_ctor());
    DECLARE_INSTANCE_FIELD_PRIVATE_DEFAULT(ListW<Zenject::MonoInstaller*>, _installerPrefabs, List<Zenject::MonoInstaller*>::New_ctor());
    DECLARE_INSTANCE_FIELD_PRIVATE_DEFAULT(ListW<UnityEngine::GameObject*>, _objectsToEnable, List<UnityEngine::GameObject*>::New_ctor());

    DECLARE_CTOR(ctor, GlobalNamespace::GameScenesManager* scenesManager);

    public:
        static EnvironmentPatcher* get_instance();

        void PreventEnvironmentInjection(Zenject::SceneDecoratorContext* instance, ListW<UnityEngine::MonoBehaviour*> monoBehaviours, Zenject::DiContainer* container);
        void PreventEnvironmentInstall(Zenject::SceneDecoratorContext* instance, ListW<Zenject::InstallerBase*> normalInstallers, ListW<System::Type*> normalInstallerTypes, ListW<Zenject::ScriptableObjectInstaller*> scriptableObjectInstallers, ListW<Zenject::MonoInstaller*> monoInstallers, ListW<Zenject::MonoInstaller*> installerPrefabs);
        void PreventEnvironmentActivation(ListW<StringW> scenesToPresent);
        void InjectEnvironment(Zenject::GameObjectContext* instance, ListW<UnityEngine::MonoBehaviour*> monoBehaviours);
        void InstallEnvironment(Zenject::Context* instance, ListW<Zenject::InstallerBase*> normalInstallers, ListW<System::Type*> normalInstallerTypes, ListW<Zenject::ScriptableObjectInstaller*> scriptableObjectInstallers, ListW<Zenject::MonoInstaller*> installers, ListW<Zenject::MonoInstaller*> installerPrefabs);
        void LoveYouCountersPlus(Zenject::GameObjectContext* instance);
        void ActivateEnvironment(Zenject::GameObjectContext* instance);
        void HideOtherPlayerPlatforms(Zenject::Context* instance);
        bool RemoveDuplicateInstalls(GlobalNamespace::EnvironmentSceneSetup* instance);
        void SetEnvironmentColors(GlobalNamespace::GameplayCoreInstaller* instance);
    private:
        static EnvironmentPatcher* instance;
)
