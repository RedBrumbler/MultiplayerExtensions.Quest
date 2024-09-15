#include "Patchers/EnvironmentPatcher.hpp"
#include "config.hpp"
#include "logging.hpp"

#include "UnityEngine/SceneManagement/SceneManager.hpp"
#include "UnityEngine/SceneManagement/Scene.hpp"
#include "UnityEngine/Transform.hpp"
#include "GlobalNamespace/LightWithIdManager.hpp"
#include "GlobalNamespace/TrackLaneRingsManager.hpp"
#include "GlobalNamespace/CoreGameHUDController.hpp"
#include "GlobalNamespace/MultiplayerPositionHUDController.hpp"
#include "GlobalNamespace/MultiplayerLocalActivePlayerFacade.hpp"
#include "GlobalNamespace/EnvironmentColorManager.hpp"
#include "GlobalNamespace/LightSwitchEventEffect.hpp"
#include "GlobalNamespace/EnvironmentBrandingManager.hpp"
#include "GlobalNamespace/ColorSO.hpp"
#include "GlobalNamespace/ColorExtensions.hpp"
#include "Tweening/ColorTween.hpp"
#include "Zenject/ConcreteIdBinderGeneric_1.hpp"
#include "Zenject/ZenjectBinding.hpp"

#include "custom-types/shared/delegate.hpp"

DEFINE_TYPE(MultiplayerExtensions::Patchers, EnvironmentPatcher);

namespace MultiplayerExtensions::Patchers {
    EnvironmentPatcher* EnvironmentPatcher::instance;
    EnvironmentPatcher* EnvironmentPatcher::get_instance() { return instance; }

    void EnvironmentPatcher::ctor(GlobalNamespace::GameScenesManager* scenesManager) {
        INVOKE_CTOR();
        instance = this;
        _scenesManager = scenesManager;
    }

    void EnvironmentPatcher::Dispose() {
        if (instance == this) instance = nullptr;
    }

    void EnvironmentPatcher::PreventEnvironmentInjection(Zenject::SceneDecoratorContext* instance, ListW<UnityEngine::MonoBehaviour*> monoBehaviours, Zenject::DiContainer* container) {
        bool inStack = _scenesManager->IsSceneInStack("MultiplayerEnvironment");
        if (config.soloEnvironment && inStack) {
            auto scene = instance->get_gameObject()->get_scene();
            auto sceneName = scene.get_name();
            INFO("Fixing bind conflicts on scene '{}'.", sceneName);
            ListW<UnityEngine::MonoBehaviour*> removedBehaviours = ListW<UnityEngine::MonoBehaviour*>::New();

            if (sceneName->Contains("Environment") && !sceneName->Contains("Multiplayer")) {
                for (auto mb : monoBehaviours) {
                    auto binding = il2cpp_utils::try_cast<Zenject::ZenjectBinding>(mb).value_or(nullptr);
                    if (binding) {
                        auto lightWithIdManagerItr = std::find_if(binding->_components.begin(), binding->_components.end(), [](auto c){ return c.template try_cast<GlobalNamespace::LightWithIdManager>().has_value(); });
                        if (lightWithIdManagerItr != binding->_components.end()) removedBehaviours->Add(mb);
                    }
                }
            }

            if (removedBehaviours.size() > 0) {
                INFO("Removing the following behaviours from scene '{}':", sceneName);
                for (auto b : removedBehaviours) {
                    INFO("\t{}", b->GetType()->ToString());
                    monoBehaviours->Remove(b);
                }
            }

            if (sceneName->Contains("Environment") && !sceneName->Contains("Multiplayer")) {
                INFO("Preventing environment injection.");
                _behavioursToInject = ListW<UnityEngine::MonoBehaviour*>::New();
                _behavioursToInject->AddRange(monoBehaviours->i___System__Collections__Generic__IEnumerable_1_T_());
                monoBehaviours->Clear();
            }
        } else {
            _behavioursToInject->Clear();
        }
    }

    void EnvironmentPatcher::PreventEnvironmentInstall(Zenject::SceneDecoratorContext* instance, ListW<Zenject::InstallerBase*> normalInstallers, ListW<System::Type*> normalInstallerTypes, ListW<Zenject::ScriptableObjectInstaller*> scriptableObjectInstallers, ListW<Zenject::MonoInstaller*> monoInstallers, ListW<Zenject::MonoInstaller*> installerPrefabs) {
        auto scene = instance->get_gameObject()->get_scene();
        auto sceneName = scene.get_name();
        bool inStack = _scenesManager->IsSceneInStack("MultiplayerEnvironment");
        if (config.soloEnvironment && inStack && sceneName->Contains("Environment") && !sceneName->Contains("Multiplayer")) {
            INFO("Preventing environment installation");
            _normalInstallers = ListW<Zenject::InstallerBase*>::New();
            _normalInstallers->AddRange(normalInstallers->i___System__Collections__Generic__IEnumerable_1_T_());
            _normalInstallerTypes = ListW<System::Type*>::New();
            _normalInstallerTypes->AddRange(normalInstallerTypes->i___System__Collections__Generic__IEnumerable_1_T_());
            _scriptableObjectInstallers = ListW<Zenject::ScriptableObjectInstaller*>::New();
            _scriptableObjectInstallers->AddRange(scriptableObjectInstallers->i___System__Collections__Generic__IEnumerable_1_T_());
            _monoInstallers = ListW<Zenject::MonoInstaller*>::New();
            _monoInstallers->AddRange(monoInstallers->i___System__Collections__Generic__IEnumerable_1_T_());
            _installerPrefabs = ListW<Zenject::MonoInstaller*>::New();
            _installerPrefabs->AddRange(installerPrefabs->i___System__Collections__Generic__IEnumerable_1_T_());

            normalInstallers->Clear();
            normalInstallerTypes->Clear();
            scriptableObjectInstallers->Clear();
            monoInstallers->Clear();
            installerPrefabs->Clear();
        } else if (!inStack) {
            _normalInstallers->Clear();
            _normalInstallerTypes->Clear();
            _scriptableObjectInstallers->Clear();
            _monoInstallers->Clear();
            _installerPrefabs->Clear();
        }
    }

    void EnvironmentPatcher::PreventEnvironmentActivation(ListW<StringW> scenesToPresent) {
        auto defaultSceneItr = std::find_if(scenesToPresent.begin(), scenesToPresent.end(), [](auto scene){ return scene->Contains("Environment") && !scene->Contains("Multiplayer"); });
        if (defaultSceneItr != scenesToPresent.end()) {
            auto defaultScene = *defaultSceneItr;
            if (scenesToPresent->Contains("MultiplayerEnvironment")) {
                INFO("Preventing environment activation. ({})", defaultScene);
                auto rootObjects = UnityEngine::SceneManagement::SceneManager::GetSceneByName(defaultScene).GetRootGameObjects();
                _objectsToEnable = ListW<UnityEngine::GameObject*>::New();
                _objectsToEnable->EnsureCapacity(rootObjects.size());
                for (auto go : rootObjects) _objectsToEnable->Add(go);

                scenesToPresent->Remove(defaultScene);
            } else {
                // Make sure hud is enabled in solo
                auto sceneObjects = UnityEngine::SceneManagement::SceneManager::GetSceneByName(defaultScene).GetRootGameObjects();
                for (auto go : sceneObjects) {
                    auto hud = go->GetComponentInChildren<GlobalNamespace::CoreGameHUDController*>();
                    if (hud)
                        hud->get_gameObject()->SetActive(true);
                }
            }
        }
    }

    void EnvironmentPatcher::InjectEnvironment(Zenject::GameObjectContext* instance, ListW<UnityEngine::MonoBehaviour*> monoBehaviours) {
        auto t = instance->get_transform();
        if (config.soloEnvironment && t->get_name()->Contains("LocalActivePlayer")) {
            INFO("Injecting Environment");

            monoBehaviours->AddRange(_behavioursToInject->i___System__Collections__Generic__IEnumerable_1_T_());
        }
    }

    void EnvironmentPatcher::InstallEnvironment(Zenject::Context* instance, ListW<Zenject::InstallerBase*> normalInstallers, ListW<System::Type*> normalInstallerTypes, ListW<Zenject::ScriptableObjectInstaller*> scriptableObjectInstallers, ListW<Zenject::MonoInstaller*> installers, ListW<Zenject::MonoInstaller*> installerPrefabs) {
        auto t = instance->get_transform();
        if (config.soloEnvironment && il2cpp_utils::try_cast<Zenject::GameObjectContext>(instance).has_value() && t->get_name()->Contains("LocalActivePlayer")) {
            INFO("Installing Environment");

            normalInstallers->AddRange(_normalInstallers->i___System__Collections__Generic__IEnumerable_1_T_());
            normalInstallerTypes->AddRange(_normalInstallerTypes->i___System__Collections__Generic__IEnumerable_1_T_());
            scriptableObjectInstallers->AddRange(_scriptableObjectInstallers->i___System__Collections__Generic__IEnumerable_1_T_());
            installers->AddRange(_monoInstallers->i___System__Collections__Generic__IEnumerable_1_T_());
            installerPrefabs->AddRange(_installerPrefabs->i___System__Collections__Generic__IEnumerable_1_T_());
        }
    }

    void EnvironmentPatcher::LoveYouCountersPlus(Zenject::GameObjectContext* instance) {
        auto t = instance->get_transform();
        if (config.soloEnvironment && t->get_name()->Contains("LocalActivePlayer")) {
            auto container = instance->get_Container();

            auto hud = std::find_if(_behavioursToInject.begin(), _behavioursToInject.end(), [](auto x){ return il2cpp_utils::try_cast<GlobalNamespace::CoreGameHUDController>(x).has_value(); });
            if (hud == _behavioursToInject.end()) ERROR("Could not find hud in behavioursToInject");
            container->Unbind<GlobalNamespace::CoreGameHUDController*>();
            container->Bind<GlobalNamespace::CoreGameHUDController*>()->FromInstance(reinterpret_cast<GlobalNamespace::CoreGameHUDController*>(*hud))->AsSingle();

            auto multiHud = instance->GetComponentInChildren<GlobalNamespace::CoreGameHUDController*>();
            multiHud->get_gameObject()->SetActive(false);
            auto multiPositionHud = instance->GetComponentInChildren<GlobalNamespace::MultiplayerPositionHUDController*>();
            auto posT = multiPositionHud->get_transform();
            auto pos = posT->get_position();
            pos.y += 0.01f;
            posT->set_position(pos);
        }
    }

    void EnvironmentPatcher::ActivateEnvironment(Zenject::GameObjectContext* instance) {
        auto t = instance->get_transform();
        if (config.soloEnvironment && t->get_name()->Contains("LocalActivePlayer")) {
            INFO("Activating Environment");
            for (auto go : _objectsToEnable) {
                go->SetActive(true);
            }

            auto activeObjects = instance->get_transform()->Find("IsActiveObjects");
            activeObjects->Find("Lasers")->get_gameObject()->SetActive(false);
            activeObjects->Find("Construction")->get_gameObject()->SetActive(false);
            activeObjects->Find("BigSmokePS")->get_gameObject()->SetActive(false);
            activeObjects->Find("DustPS")->get_gameObject()->SetActive(false);
            activeObjects->Find("DirectionalLights")->get_gameObject()->SetActive(false);

            auto localActivePlayer = t->GetComponent<GlobalNamespace::MultiplayerLocalActivePlayerFacade*>();
            auto activeOnlyGameObjects = localActivePlayer->_activeOnlyGameObjects;
            ListW<UnityW<UnityEngine::GameObject>> newActiveOnlyGameObjects = ListW<UnityW<UnityEngine::GameObject>>::New();
            newActiveOnlyGameObjects->EnsureCapacity(activeOnlyGameObjects.size() + _objectsToEnable.size());
            for (auto go : activeOnlyGameObjects) newActiveOnlyGameObjects->Add(go);
            for (auto go : _objectsToEnable) newActiveOnlyGameObjects->Add(go);
            localActivePlayer->_activeOnlyGameObjects = newActiveOnlyGameObjects->ToArray();
        }
    }

    void EnvironmentPatcher::HideOtherPlayerPlatforms(Zenject::Context* instance) {
        auto t = instance->get_transform();
        if (t->get_name()->Contains("ConnectedPlayer")) {
            if (config.disableMultiplayerPlatforms)
                t->Find("Construction")->get_gameObject()->SetActive(false);
            if (config.disableMultiplayerLights)
                t->Find("Lasers")->get_gameObject()->SetActive(false);
        }
    }

    bool EnvironmentPatcher::RemoveDuplicateInstalls(GlobalNamespace::EnvironmentSceneSetup* instance) {
        auto container = instance->Container;
        return !container->HasBinding<GlobalNamespace::EnvironmentBrandingManager::InitData*>();
    }

    void EnvironmentPatcher::SetEnvironmentColors(GlobalNamespace::GameplayCoreInstaller* instance) {
        if (!config.soloEnvironment || !_scenesManager->IsSceneInStack("MultiplayerEnvironment")) return;

        auto container = instance->Container;
        auto colorManager = container->Resolve<GlobalNamespace::EnvironmentColorManager*>();
        container->Inject(colorManager);
        colorManager->Awake();

        for (auto go : _objectsToEnable) {
            auto lightSwitchEventEffects = go->transform->GetComponentsInChildren<GlobalNamespace::LightSwitchEventEffect*>();
            if (!lightSwitchEventEffects) {
                WARNING("Could not get LightSwitchEventEffect, continuing");
                continue;
            }
            
            for (auto light : lightSwitchEventEffects) {
                // We have to set this manually since BG moved the below into Start() which we can't call without causing a nullref
                light->_usingBoostColors = false;
                auto color = (light->_lightOnStart ? light->_lightColor0->color : GlobalNamespace::ColorExtensions::ColorWithAlpha(light->_lightColor0->color, light->_offColorIntensity));
                auto color2 = (light->_lightOnStart ? light->_lightColor0Boost->color : GlobalNamespace::ColorExtensions::ColorWithAlpha(light->_lightColor0Boost->color, light->_offColorIntensity));
                light->_colorTween = Tweening::ColorTween::New_ctor(color, color, custom_types::MakeDelegate<System::Action_1<UnityEngine::Color>*>(light, (std::function<void(GlobalNamespace::LightSwitchEventEffect*, UnityEngine::Color)>)&GlobalNamespace::LightSwitchEventEffect::SetColor), 0.0f, GlobalNamespace::EaseType::Linear, 0.0f);
                light->SetupTweenAndSaveOtherColors(color, color, color2, color2);
            }
        }
    }
}
