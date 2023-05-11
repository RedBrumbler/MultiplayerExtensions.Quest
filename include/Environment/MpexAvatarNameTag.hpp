#pragma once

#include "custom-types/shared/macros.hpp"
#include "lapiz/shared/macros.hpp"

#include "UnityEngine/MonoBehaviour.hpp"
#include "HMUI/ImageView.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"
#include "System/Collections/Generic/Dictionary_2.hpp"
#include "GlobalNamespace/IConnectedPlayer.hpp"


#include "multiplayer-core/shared/Players/MpPlayerManager.hpp"
#include "PlayerIconSlot.hpp"
#include "Players/MpexPlayerManager.hpp"
#include "Players/MpexPlayerData.hpp"
#include "Utilities/SpriteManager.hpp"

using IconDict = System::Collections::Generic::Dictionary_2<MultiplayerExtensions::Environment::PlayerIconSlot, HMUI::ImageView*>;

DECLARE_CLASS_CODEGEN(MultiplayerExtensions::Environment, MpexAvatarNameTag, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD_PRIVATE(IconDict*, _playerIcons);
    DECLARE_INSTANCE_FIELD_PRIVATE(GlobalNamespace::IConnectedPlayer*, _player);
    DECLARE_INSTANCE_FIELD_PRIVATE(MultiplayerCore::Players::MpPlayerManager*, _playerManager);
    DECLARE_INSTANCE_FIELD_PRIVATE(Players::MpexPlayerManager*, _mpexPlayerManager);
    DECLARE_INSTANCE_FIELD_PRIVATE(Utilities::SpriteManager*, _spriteManager);

    DECLARE_INSTANCE_FIELD_PRIVATE(HMUI::ImageView*, _bg);
    DECLARE_INSTANCE_FIELD_PRIVATE(HMUI::CurvedTextMeshPro*, _nameText);

    DECLARE_INJECT_METHOD(void, Inject, GlobalNamespace::IConnectedPlayer* player, MultiplayerCore::Players::MpPlayerManager* playerManager, Players::MpexPlayerManager* mpexPlayerManager, Utilities::SpriteManager* spriteManager);

    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, OnEnable);
    DECLARE_INSTANCE_METHOD(void, OnDisable);

    DECLARE_CTOR(ctor);
    public:
        void HandlePlatformData(GlobalNamespace::IConnectedPlayer* player, MultiplayerCore::Players::MpPlayerData* data);
        void HandleMpexData(GlobalNamespace::IConnectedPlayer* player, Players::MpexPlayerData* data);
        void SetPlatformData(MultiplayerCore::Players::MpPlayerData* data);
        void SetIcon(PlayerIconSlot slot, UnityEngine::Sprite* icon);

        HMUI::ImageView* CreateIcon();
)
