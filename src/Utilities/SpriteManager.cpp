#include "Utilities/SpriteManager.hpp"

#include "assets.hpp"
#include "bsml/shared/Helpers/utilities.hpp"
DEFINE_TYPE(MultiplayerExtensions::Utilities, SpriteManager);

#define CLEAN_SPRITE(sprite) \
    if (sprite && sprite->m_CachedPtr)\
        UnityEngine::Object::Destroy(sprite);\
    sprite = nullptr

namespace MultiplayerExtensions::Utilities {
    void SpriteManager::Initialize() {
        _oculusIcon = BSML::Utilities::LoadSpriteRaw(Assets::Icons::Oculus64_png);
        _steamIcon = BSML::Utilities::LoadSpriteRaw(Assets::Icons::Steam64_png);
        _metaIcon = BSML::Utilities::LoadSpriteRaw(Assets::Icons::Meta64_png);
        _toasterIcon = BSML::Utilities::LoadSpriteRaw(Assets::Icons::Toaster64_png);
    }

    void SpriteManager::Dispose() {
        CLEAN_SPRITE(_oculusIcon);
        CLEAN_SPRITE(_steamIcon);
        CLEAN_SPRITE(_metaIcon);
        CLEAN_SPRITE(_toasterIcon);
    }
}
