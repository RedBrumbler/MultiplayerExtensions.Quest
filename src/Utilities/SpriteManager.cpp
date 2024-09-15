#include "Utilities/SpriteManager.hpp"

#include "assets.hpp"
#include "bsml/shared/Helpers/utilities.hpp"
DEFINE_TYPE(MultiplayerExtensions::Utilities, SpriteManager);

#define CLEAN_SPRITE(sprite) \
    if (sprite && sprite->m_CachedPtr.m_value)\
        UnityEngine::Object::Destroy(sprite);\
    sprite = nullptr
namespace MultiplayerExtensions::Utilities {
    void SpriteManager::Initialize() {
        _oculusIcon = BSML::Utilities::LoadSpriteRaw(Assets::IconOculus64_png);
        _steamIcon = BSML::Utilities::LoadSpriteRaw(Assets::IconSteam64_png);
        _metaIcon = BSML::Utilities::LoadSpriteRaw(Assets::IconMeta64_png);
        _toasterIcon = BSML::Utilities::LoadSpriteRaw(Assets::IconToaster64_png);
    }

    void SpriteManager::Dispose() {
        CLEAN_SPRITE(_oculusIcon);
        CLEAN_SPRITE(_steamIcon);
        CLEAN_SPRITE(_metaIcon);
        CLEAN_SPRITE(_toasterIcon);
    }
}
