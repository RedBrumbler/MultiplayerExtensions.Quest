#include "Environment/MpexAvatarNameTag.hpp"

#include "bsml/shared/Helpers/utilities.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/CanvasRenderer.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"
#include "GlobalNamespace/ConnectedPlayerName.hpp"

DEFINE_TYPE(MultiplayerExtensions::Environment, MpexAvatarNameTag);

namespace MultiplayerExtensions::Environment {
    void MpexAvatarNameTag::ctor() {
        INVOKE_BASE_CTOR(classof(UnityEngine::MonoBehaviour*));
        _playerIcons = IconDict::New_ctor();
    }

    void MpexAvatarNameTag::Inject(GlobalNamespace::IConnectedPlayer* player, MultiplayerCore::Players::MpPlayerManager* playerManager, Players::MpexPlayerManager* mpexPlayerManager, Utilities::SpriteManager* spriteManager) {
        _player = player;
        _playerManager = playerManager;
        _mpexPlayerManager = mpexPlayerManager;
        _spriteManager = spriteManager;
    }

    void MpexAvatarNameTag::Awake() {
        auto t = get_transform();
        _bg = t->Find("BG")->GetComponent<HMUI::ImageView*>();
        _nameText = t->Find("Name")->GetComponent<HMUI::CurvedTextMeshPro*>();

        if (!_bg->GetComponent<UnityEngine::UI::HorizontalLayoutGroup*>()) {
            auto layout = _bg->GetComponent<UnityEngine::UI::HorizontalLayoutGroup*>();
            layout->set_childAlignment(UnityEngine::TextAnchor::MiddleCenter);
            layout->set_childForceExpandWidth(false);
            layout->set_childForceExpandHeight(false);
            layout->set_childScaleWidth(false);
            layout->set_childScaleHeight(false);
            layout->set_spacing(4.0f);
        }

        _nameText->get_transform()->SetParent(_bg->get_transform(), false);

        if (auto nativeNameScript = _nameText->GetComponent<GlobalNamespace::ConnectedPlayerName*>()) {
            UnityEngine::Object::DestroyImmediate(nativeNameScript);
        }
        _nameText->set_text(_player->get_userName());
        _nameText->set_color({1, 1, 1, 1});

        MultiplayerExtensions::Players::MpexPlayerData* mpexData = nullptr;
        if (_mpexPlayerManager->TryGetPlayer(_player->get_userId(), mpexData))
            _nameText->set_color(mpexData->playerColor);

        MultiplayerCore::Players::MpPlayerData* mpData = nullptr;
        if (_playerManager->TryGetPlayer(_player->get_userId(), mpData))
            SetPlatformData(mpData);
    }

    void MpexAvatarNameTag::OnEnable() {
        _playerManager->PlayerConnectedEvent += {&MpexAvatarNameTag::HandlePlatformData, this};
        _mpexPlayerManager->PlayerConnectedEvent += {&MpexAvatarNameTag::HandleMpexData, this};
    }

    void MpexAvatarNameTag::OnDisable() {
        _playerManager->PlayerConnectedEvent -= {&MpexAvatarNameTag::HandlePlatformData, this};
        _mpexPlayerManager->PlayerConnectedEvent -= {&MpexAvatarNameTag::HandleMpexData, this};
    }

    void MpexAvatarNameTag::HandlePlatformData(GlobalNamespace::IConnectedPlayer* player, MultiplayerCore::Players::MpPlayerData* data) {
        if (player == _player)
            SetPlatformData(data);
    }

    void MpexAvatarNameTag::HandleMpexData(GlobalNamespace::IConnectedPlayer* player, Players::MpexPlayerData* data) {
        if (player == _player)
            _nameText->set_color(data->playerColor);
    }

    void MpexAvatarNameTag::SetPlatformData(MultiplayerCore::Players::MpPlayerData* data) {
        UnityEngine::Sprite* icon = nullptr;
        switch(data->platform) {
            case MultiplayerCore::Players::Platform::Steam:
                icon = _spriteManager->get_steamIcon();
                break;
            case MultiplayerCore::Players::Platform::OculusPC:
                icon = _spriteManager->get_oculusIcon();
                break;
            case MultiplayerCore::Players::Platform::OculusQuest:
                icon = _spriteManager->get_metaIcon();
                break;
            default:
                ERROR("Sprite for platform {} is not available!", (int)data->platform);
                icon = _spriteManager->get_toasterIcon();
                break;
        }
        SetIcon(PlayerIconSlot::Platform, icon);
    }

    void MpexAvatarNameTag::SetIcon(PlayerIconSlot slot, UnityEngine::Sprite* icon) {
        HMUI::ImageView* imageView = nullptr;
        if (!_playerIcons->TryGetValue(slot, byref(imageView))) {
            imageView = CreateIcon();
            imageView->get_transform()->SetSiblingIndex((int)slot);
            imageView->get_gameObject()->set_name(fmt::format("MpexPlayerIcon({})", slot));
            _playerIcons->Add(slot, imageView);
        }

        imageView->set_sprite(icon);
        _nameText->get_transform()->SetAsLastSibling();
    }

    HMUI::ImageView* MpexAvatarNameTag::CreateIcon() {
        auto iconObj = UnityEngine::GameObject::New_ctor();
        auto t = iconObj->get_transform();
        t->SetParent(_bg->get_transform(), false);
        t->set_localScale({3.2f, 3.2f, 3.2f});
        iconObj->set_layer(5);

        iconObj->AddComponent<UnityEngine::CanvasRenderer*>();

        auto imageView = iconObj->AddComponent<HMUI::ImageView*>();
        imageView->set_maskable(true);
        imageView->set_fillCenter(true);
        imageView->set_preserveAspect(true);
        imageView->set_material(_bg->get_material());

        return imageView;
    }
}
