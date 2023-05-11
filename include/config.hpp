#pragma once

#include "UnityEngine/Color.hpp"

struct Config {
    static UnityEngine::Color defaultPlayerColor;
    bool soloEnvironment = false;

    bool sideBySide = false;
    float sideBySideDistance = 4.0f;

    bool disableMultiplayerPlatforms = false;
    bool disableMultiplayerLights = false;
    bool disableMultiplayerObjects = false;
    bool disableMultiplayerColors = false;
    bool disablePlatformMovement = false;
    bool missLighting = false;
    bool personalMissLightingOnly = false;

    UnityEngine::Color playerColor = defaultPlayerColor;
    UnityEngine::Color missColor = UnityEngine::Color(1, 0, 0, 1);
};

extern Config config;
void SaveConfig();
bool LoadConfig();
