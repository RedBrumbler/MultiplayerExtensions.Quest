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
using namespace System::Collections::Generic;

MAKE_AUTO_HOOK_MATCH(SceneDecoratorContext_GetInjectableMonoBehaviours, &SceneDecoratorContext::GetInjectableMonoBehaviours, void, SceneDecoratorContext* self, List_1<::UnityW<::UnityEngine::MonoBehaviour>>* monoBehaviours) {
    SceneDecoratorContext_GetInjectableMonoBehaviours(self, monoBehaviours);
    // postfix
    auto patcher = Patchers::EnvironmentPatcher::get_instance();
    if (patcher) {
        patcher->PreventEnvironmentInjection(self, monoBehaviours, self->_container);
    }
}

MAKE_AUTO_HOOK_MATCH(GameScenesManager_ActivatePresentedSceneRootObjects, &GameScenesManager::ActivatePresentedSceneRootObjects, void, List_1<StringW>* scenesToPresent) {
    auto patcher = Patchers::EnvironmentPatcher::get_instance();
    if (patcher) {
        // prefix
        patcher->PreventEnvironmentActivation(scenesToPresent);
    }

    GameScenesManager_ActivatePresentedSceneRootObjects(scenesToPresent);
}

MAKE_AUTO_HOOK_MATCH(GameObjectContext_GetInjectableMonoBehaviours, &GameObjectContext::GetInjectableMonoBehaviours, void, GameObjectContext* self, List_1<::UnityW<::UnityEngine::MonoBehaviour>>* monoBehaviours) {
    GameObjectContext_GetInjectableMonoBehaviours(self, monoBehaviours);

    auto patcher = Patchers::EnvironmentPatcher::get_instance();
    if (patcher) {
        // postfix
        patcher->InjectEnvironment(self, monoBehaviours);
    }
}

MAKE_AUTO_HOOK_MATCH(Context_InstallInstallers_5, static_cast<void (Context::*)(
                                List_1<::Zenject::InstallerBase*>*, 
                                List_1<::System::Type*>*,
                                List_1<::UnityW<::Zenject::ScriptableObjectInstaller>>*,
                                List_1<::UnityW<::Zenject::MonoInstaller>>*,
                                List_1<::UnityW<::Zenject::MonoInstaller>>*)>(&Context::InstallInstallers), 
                                void, Context* self, 
                                List_1<::Zenject::InstallerBase*>* normalInstallers, 
                                List_1<::System::Type*>* normalInstallerTypes,
                                List_1<::UnityW<::Zenject::ScriptableObjectInstaller>>* scriptableObjectInstallers,
                                List_1<::UnityW<::Zenject::MonoInstaller>>* installers,
                                List_1<::UnityW<::Zenject::MonoInstaller>>* installerPrefabs) {
    auto patcher = Patchers::EnvironmentPatcher::get_instance();
    if (patcher) {
        // prefix
        if (il2cpp_utils::try_cast<GameObjectContext>(self).has_value())
            patcher->LoveYouCountersPlus(reinterpret_cast<GameObjectContext*>(self));
        else if (il2cpp_utils::try_cast<SceneDecoratorContext>(self).has_value())
            patcher->PreventEnvironmentInstall(reinterpret_cast<SceneDecoratorContext*>(self), self->_normalInstallers, self->_normalInstallerTypes, self->_scriptableObjectInstallers, self->_monoInstallers, self->_installerPrefabs);

        patcher->InstallEnvironment(self, normalInstallers, normalInstallerTypes, scriptableObjectInstallers, installerPrefabs, installerPrefabs);
    }

    Context_InstallInstallers_5(self, normalInstallers, normalInstallerTypes, scriptableObjectInstallers, installers, installerPrefabs);
}

MAKE_AUTO_HOOK_MATCH(Context_InstallSceneBindings, &Context::InstallSceneBindings, void, Context* self, List_1<UnityW<MonoBehaviour>>* injectableMonoBehaviours) {
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
