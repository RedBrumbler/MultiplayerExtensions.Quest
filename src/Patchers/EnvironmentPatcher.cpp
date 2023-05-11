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
#include "GlobalNamespace/EnvironmentBrandingManager_InitData.hpp"
#include "Zenject/ConcreteIdBinderGeneric_1.hpp"
#include "Zenject/ZenjectBinding.hpp"

DEFINE_TYPE(MultiplayerExtensions::Patchers, EnvironmentPatcher);

namespace MultiplayerExtensions::Patchers {
    EnvironmentPatcher* EnvironmentPatcher::instance;
    EnvironmentPatcher* EnvironmentPatcher::get_instance() { return instance; }

    void EnvironmentPatcher::ctor(GlobalNamespace::GameScenesManager* scenesManager) {
        INVOKE_CTOR();
        _scenesManager = scenesManager;
    }

    void EnvironmentPatcher::Dispose() {
        if (instance == this) instance = nullptr;
    }

    void EnvironmentPatcher::PreventEnvironmentInjection(Zenject::SceneDecoratorContext* instance, ListW<UnityEngine::MonoBehaviour*> monoBehaviours, Zenject::DiContainer* container) {
        auto scene = instance->get_gameObject()->get_scene();
        if (config.soloEnvironment && _scenesManager->IsSceneInStack("MultiplayerEnvironment")) {
            auto sceneName = scene.get_name();
            INFO("Fixing bind conflicts on scene '{}'.", sceneName);
            ListW<UnityEngine::MonoBehaviour*> removedBehaviours = List<UnityEngine::MonoBehaviour*>::New_ctor();

            if (sceneName->Contains("Environment") && !sceneName->Contains("Multiplayer")) {
                for (auto mb : monoBehaviours) {
                    auto binding = il2cpp_utils::try_cast<Zenject::ZenjectBinding>(mb).value_or(nullptr);
                    if (binding) {
                        for (auto c : binding->components) {
                            if (il2cpp_utils::try_cast<GlobalNamespace::LightWithIdManager>(c).has_value()) {
                                removedBehaviours->Add(mb);
                                break;
                            }
                        }
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
                _behavioursToInject = List<UnityEngine::MonoBehaviour*>::New_ctor();
                _behavioursToInject->AddRange(monoBehaviours->i_IReadOnlyList_1_T()->i_IReadOnlyCollection_1_T()->i_IEnumerable_1_T());
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
            _normalInstallers = List<Zenject::InstallerBase*>::New_ctor();
            _normalInstallers->AddRange(normalInstallers->i_IReadOnlyList_1_T()->i_IReadOnlyCollection_1_T()->i_IEnumerable_1_T());
            _normalInstallerTypes = List<System::Type*>::New_ctor();
            _normalInstallerTypes->AddRange(normalInstallerTypes->i_IReadOnlyList_1_T()->i_IReadOnlyCollection_1_T()->i_IEnumerable_1_T());
            _scriptableObjectInstallers = List<Zenject::ScriptableObjectInstaller*>::New_ctor();
            _scriptableObjectInstallers->AddRange(scriptableObjectInstallers->i_IReadOnlyList_1_T()->i_IReadOnlyCollection_1_T()->i_IEnumerable_1_T());
            _monoInstallers = List<Zenject::MonoInstaller*>::New_ctor();
            _monoInstallers->AddRange(monoInstallers->i_IReadOnlyList_1_T()->i_IReadOnlyCollection_1_T()->i_IEnumerable_1_T());
            _installerPrefabs = List<Zenject::MonoInstaller*>::New_ctor();
            _installerPrefabs->AddRange(installerPrefabs->i_IReadOnlyList_1_T()->i_IReadOnlyCollection_1_T()->i_IEnumerable_1_T());

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
                _objectsToEnable = List<UnityEngine::GameObject*>::New_ctor();
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

            monoBehaviours->AddRange(_behavioursToInject->i_IReadOnlyList_1_T()->i_IReadOnlyCollection_1_T()->i_IEnumerable_1_T());
        }
    }

    void EnvironmentPatcher::InstallEnvironment(Zenject::Context* instance, ListW<Zenject::InstallerBase*> normalInstallers, ListW<System::Type*> normalInstallerTypes, ListW<Zenject::ScriptableObjectInstaller*> scriptableObjectInstallers, ListW<Zenject::MonoInstaller*> installers, ListW<Zenject::MonoInstaller*> installerPrefabs) {
        auto t = instance->get_transform();
        if (config.soloEnvironment && il2cpp_utils::try_cast<Zenject::GameObjectContext>(instance).has_value() && t->get_name()->Contains("LocalActivePlayer")) {
            INFO("Installing Environment");

            normalInstallers->AddRange(_normalInstallers->i_IReadOnlyList_1_T()->i_IReadOnlyCollection_1_T()->i_IEnumerable_1_T());
            normalInstallerTypes->AddRange(_normalInstallerTypes->i_IReadOnlyList_1_T()->i_IReadOnlyCollection_1_T()->i_IEnumerable_1_T());
            scriptableObjectInstallers->AddRange(_scriptableObjectInstallers->i_IReadOnlyList_1_T()->i_IReadOnlyCollection_1_T()->i_IEnumerable_1_T());
            installers->AddRange(_monoInstallers->i_IReadOnlyList_1_T()->i_IReadOnlyCollection_1_T()->i_IEnumerable_1_T());
            installerPrefabs->AddRange(_installerPrefabs->i_IReadOnlyList_1_T()->i_IReadOnlyCollection_1_T()->i_IEnumerable_1_T());
        }
    }

    void EnvironmentPatcher::LoveYouCountersPlus(Zenject::GameObjectContext* instance) {
        auto t = instance->get_transform();
        if (config.soloEnvironment && t->get_name()->Contains("LocalActivePlayer")) {
            auto container = instance->get_Container();

            auto hud = std::find_if(_behavioursToInject.begin(), _behavioursToInject.end(), [](auto x){ return il2cpp_utils::try_cast<GlobalNamespace::CoreGameHUDController>(x).has_value(); });
            if (hud == _behavioursToInject.end()) ERROR("Could not find hud in behavioursToInject");
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
            auto activeOnlyGameObjects = localActivePlayer->activeOnlyGameObjects;
            ListW<UnityEngine::GameObject*> newActiveOnlyGameObjects = List<UnityEngine::GameObject*>::New_ctor();
            newActiveOnlyGameObjects->EnsureCapacity(activeOnlyGameObjects.size() + _objectsToEnable.size());
            for (auto go : activeOnlyGameObjects) newActiveOnlyGameObjects->Add(go);
            for (auto go : _objectsToEnable) newActiveOnlyGameObjects->Add(go);
            localActivePlayer->activeOnlyGameObjects = newActiveOnlyGameObjects->ToArray();
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
        auto container = instance->get_Container();
        return !container->HasBinding<GlobalNamespace::EnvironmentBrandingManager::InitData*>();
    }

    void EnvironmentPatcher::SetEnvironmentColors(GlobalNamespace::GameplayCoreInstaller* instance) {
        if (!config.soloEnvironment || !_scenesManager->IsSceneInStack("MultiplayerEnvironment")) return;

        auto container = instance->get_Container();
        auto colorManager = container->Resolve<GlobalNamespace::EnvironmentColorManager*>();
        container->Inject(colorManager);
        colorManager->Awake();
        colorManager->Start();

        for (auto go : _objectsToEnable) {
            auto lightSwitchEventEffects = go->GetComponentsInChildren<GlobalNamespace::LightSwitchEventEffect*>();
            for (auto light : lightSwitchEventEffects) light->Awake();
        }
    }
}
