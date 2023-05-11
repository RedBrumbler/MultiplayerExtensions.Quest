#include "Utilities/SpriteManager.hpp"

#include "assets.hpp"
#include "bsml/shared/Helpers/utilities.hpp"
DEFINE_TYPE(MultiplayerExtensions::Utilities, SpriteManager);

namespace MultiplayerExtensions::Utilities {
    void SpriteManager::Initialize() {
        _oculusIcon = BSML::Utilities::LoadSpriteRaw(IncludedAssets::IconOculus64_png);
        _steamIcon = BSML::Utilities::LoadSpriteRaw(IncludedAssets::IconSteam64_png);
    }

    void SpriteManager::Dispose() {
        if (_oculusIcon && _oculusIcon->m_CachedPtr.m_value)
            UnityEngine::Object::Destroy(_oculusIcon);
        _oculusIcon = nullptr;
        if (_steamIcon && _steamIcon->m_CachedPtr.m_value)
            UnityEngine::Object::Destroy(_steamIcon);
        _steamIcon = nullptr;
    }
}
