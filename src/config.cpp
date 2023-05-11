#include "config.hpp"
#include "logging.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"

extern ModInfo modInfo;

UnityEngine::Color Config::defaultPlayerColor(0.031f, 0.752f, 1.0f, 1.0f);

Config config;

Configuration& get_config() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

#define ADD_MEMBER(member) cfg.AddMember(#member, config.member, allocator)
#define ADD_COLOR(color) \
    rapidjson::Value color##Value;\
    color##Value.SetObject();\
    color##Value.AddMember("r", config.color.r, allocator);\
    color##Value.AddMember("g", config.color.g, allocator);\
    color##Value.AddMember("b", config.color.b, allocator);\
    color##Value.AddMember("a", config.color.a, allocator);\
    cfg.AddMember(#color, color##Value, allocator)

void SaveConfig() {
    INFO("Saving Config...");
    auto& cfg = get_config().config;
    cfg.RemoveAllMembers();
    cfg.SetObject();
    auto& allocator = cfg.GetAllocator();

    ADD_MEMBER(soloEnvironment);
    ADD_MEMBER(sideBySide);
    ADD_MEMBER(sideBySideDistance);
    ADD_MEMBER(disableMultiplayerPlatforms);
    ADD_MEMBER(disableMultiplayerLights);
    ADD_MEMBER(disableMultiplayerObjects);
    ADD_MEMBER(disableMultiplayerColors);
    ADD_MEMBER(disablePlatformMovement);
    ADD_MEMBER(missLighting);
    ADD_MEMBER(personalMissLightingOnly);

    ADD_COLOR(playerColor);
    ADD_COLOR(missColor);

    get_config().Write();

    INFO("Config Saved!");
}

#define GET_BOOL(identifier) \
    auto identifier##Itr = cfg.FindMember(#identifier);\
    if (identifier##Itr != cfg.MemberEnd() && identifier##Itr->value.IsBool())\
        config.identifier = identifier##Itr->value.GetBool();\
    else foundEverything = false

#define GET_FLOAT(identifier) \
    auto identifier##Itr = cfg.FindMember(#identifier);\
    if (identifier##Itr != cfg.MemberEnd() && identifier##Itr->value.IsFloat())\
        config.identifier = identifier##Itr->value.GetFloat();\
    else foundEverything = false

#define GET_COLOR(identifier) \
    auto identifier##Itr = cfg.FindMember(#identifier);\
    if (identifier##Itr != cfg.MemberEnd() && identifier##Itr->value.IsObject()) {\
        auto& identifier = identifier##Itr->value;\
        config.identifier.r = identifier["r"].GetFloat();\
        config.identifier.g = identifier["g"].GetFloat();\
        config.identifier.b = identifier["b"].GetFloat();\
        config.identifier.a = identifier["a"].GetFloat();\
    } else foundEverything = false

bool LoadConfig() {
    INFO("Loading Config...");
    bool foundEverything = true;
    auto& cfg = get_config().config;

    GET_BOOL(soloEnvironment);
    GET_BOOL(sideBySide);
    GET_FLOAT(sideBySideDistance);
    GET_BOOL(disableMultiplayerPlatforms);
    GET_BOOL(disableMultiplayerLights);
    GET_BOOL(disableMultiplayerObjects);
    GET_BOOL(disableMultiplayerColors);
    GET_BOOL(disablePlatformMovement);
    GET_BOOL(missLighting);
    GET_BOOL(personalMissLightingOnly);

    GET_COLOR(playerColor);
    GET_COLOR(missColor);

    if (foundEverything) INFO("Config Loaded!");
    return foundEverything;
}
