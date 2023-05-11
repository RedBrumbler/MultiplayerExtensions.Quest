#include "hooking.hpp"
#include "logging.hpp"

#include "Patchers/EnvironmentPatcher.hpp"

#include "Zenject/SceneDecoratorContext.hpp"
#include "Zenject/GameObjectContext.hpp"
#include "Zenject/Context.hpp"
#include "Zenject/InstallerBase.hpp"
#include "Zenject/ScriptableObjectInstaller.hpp"
#include "Zenject/MonoInstaller.hpp"
#include "System/Type.hpp"
#include "GlobalNamespace/GameScenesManager.hpp"
#include "GlobalNamespace/EnvironmentSceneSetup.hpp"
#include "GlobalNamespace/GameplayCoreInstaller.hpp"

using namespace MultiplayerExtensions;
using namespace UnityEngine;
using namespace GlobalNamespace;
using namespace Zenject;

MAKE_AUTO_HOOK_MATCH(SceneDecoratorContext_GetInjectableMonoBehaviours, &SceneDecoratorContext::GetInjectableMonoBehaviours, void, SceneDecoratorContext* self, List<MonoBehaviour*>* monoBehaviours) {
    SceneDecoratorContext_GetInjectableMonoBehaviours(self, monoBehaviours);
    // postfix
    auto patcher = Patchers::EnvironmentPatcher::get_instance();
    if (patcher) {
        patcher->PreventEnvironmentInjection(self, monoBehaviours, self->container);
    }
}

MAKE_AUTO_HOOK_MATCH(SceneDecoratorContext_InstallDecoratorInstallers, &SceneDecoratorContext::InstallDecoratorInstallers, void, SceneDecoratorContext* self) {
    auto patcher = Patchers::EnvironmentPatcher::get_instance();
    if (patcher) {
        // prefix
        patcher->PreventEnvironmentInstall(self, self->normalInstallers, self->normalInstallerTypes, self->scriptableObjectInstallers, self->monoInstallers, self->installerPrefabs);
    }

    SceneDecoratorContext_InstallDecoratorInstallers(self);
}

MAKE_AUTO_HOOK_MATCH(GameScenesManager_ActivatePresentedSceneRootObjects, &GameScenesManager::ActivatePresentedSceneRootObjects, void, List<StringW>* scenesToPresent) {
    auto patcher = Patchers::EnvironmentPatcher::get_instance();
    if (patcher) {
        // prefix
        patcher->PreventEnvironmentActivation(scenesToPresent);
    }

    GameScenesManager_ActivatePresentedSceneRootObjects(scenesToPresent);
}

MAKE_AUTO_HOOK_MATCH(GameObjectContext_GetInjectableMonoBehaviours, &GameObjectContext::GetInjectableMonoBehaviours, void, GameObjectContext* self, List<MonoBehaviour*>* monoBehaviours) {
    GameObjectContext_GetInjectableMonoBehaviours(self, monoBehaviours);

    auto patcher = Patchers::EnvironmentPatcher::get_instance();
    if (patcher) {
        // postfix
        patcher->InjectEnvironment(self, monoBehaviours);
    }
}

MAKE_AUTO_HOOK_MATCH(Context_InstallInstallers_5, static_cast<void (Context::*)(List<InstallerBase*>*, List<::System::Type*>*, List<ScriptableObjectInstaller*>*, List<MonoInstaller*>*, List<MonoInstaller*>*)>(&Context::InstallInstallers), void, Context* self, List<InstallerBase*>* normalInstallers, List<::System::Type*>* normalInstallerTypes, List<ScriptableObjectInstaller*>* scriptableObjectInstallers, List<MonoInstaller*>* installers, List<MonoInstaller*>* installerPrefabs) {
    auto patcher = Patchers::EnvironmentPatcher::get_instance();
    if (patcher) {
        // prefix
        patcher->InstallEnvironment(self, normalInstallers, normalInstallerTypes, scriptableObjectInstallers, installerPrefabs, installerPrefabs);
    }

    Context_InstallInstallers_5(self, normalInstallers, normalInstallerTypes, scriptableObjectInstallers, installers, installerPrefabs);
}

MAKE_AUTO_HOOK_MATCH(Context_InstallInstallers_0, static_cast<void (Context::*)()>(&Context::InstallInstallers), void, Context* self) {
    auto patcher = Patchers::EnvironmentPatcher::get_instance();
    if (patcher && il2cpp_utils::try_cast<Zenject::GameObjectContext>(self).has_value()) {
        // prefix
        patcher->LoveYouCountersPlus(reinterpret_cast<Zenject::GameObjectContext*>(self));
    }
    Context_InstallInstallers_0(self);
}

// TODO: check if self is a GameObjectContext && combine the patches
MAKE_AUTO_HOOK_MATCH(Context_InstallSceneBindings, &Context::InstallSceneBindings, void, Context* self, List<MonoBehaviour*>* injectableMonoBehaviours) {
    Context_InstallSceneBindings(self, injectableMonoBehaviours);

    auto patcher = Patchers::EnvironmentPatcher::get_instance();
    if (patcher) {
        // postfix
        patcher->HideOtherPlayerPlatforms(self);
        auto gameObjectContextSelf = il2cpp_utils::try_cast<Zenject::GameObjectContext>(self).value_or(nullptr);
        if (gameObjectContextSelf) patcher->ActivateEnvironment(gameObjectContextSelf);
    }
}

MAKE_AUTO_HOOK_MATCH(EnvironmentSceneSetup_InstallBindings, &EnvironmentSceneSetup::InstallBindings, void, EnvironmentSceneSetup* self) {
    auto patcher = Patchers::EnvironmentPatcher::get_instance();
    if (patcher) {
        // prefix & don't run orig if return false
        if (patcher->RemoveDuplicateInstalls(self))
            EnvironmentSceneSetup_InstallBindings(self);
    } else {
        EnvironmentSceneSetup_InstallBindings(self);
    }
}

MAKE_AUTO_HOOK_MATCH(GameplayCoreInstaller_InstallBindings, &GameplayCoreInstaller::InstallBindings, void, GameplayCoreInstaller* self) {
    GameplayCoreInstaller_InstallBindings(self);

    auto patcher = Patchers::EnvironmentPatcher::get_instance();
    if (patcher) {
        // postfix
        patcher->SetEnvironmentColors(self);
    }
}
