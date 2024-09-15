#pragma once

#include "custom-types/shared/macros.hpp"
#include "UnityEngine/Color.hpp"

#include "LiteNetLib/Utils/NetDataReader.hpp"
#include "LiteNetLib/Utils/NetDataWriter.hpp"
#include "LiteNetLib/Utils/INetSerializable.hpp"

DECLARE_CLASS_CODEGEN_INTERFACES(MultiplayerExtensions::Players, MpexPlayerData, Il2CppObject, classof(LiteNetLib::Utils::INetSerializable*),
    DECLARE_INSTANCE_FIELD(UnityEngine::Color, playerColor);

    DECLARE_OVERRIDE_METHOD_MATCH(void, Serialize, &LiteNetLib::Utils::INetSerializable::Serialize, LiteNetLib::Utils::NetDataWriter* writer);
    DECLARE_OVERRIDE_METHOD_MATCH(void, Deserialize, &LiteNetLib::Utils::INetSerializable::Deserialize, LiteNetLib::Utils::NetDataReader* reader);
    DECLARE_CTOR(ctor);

    public:
        LiteNetLib::Utils::INetSerializable* i_INetSerializable() { return reinterpret_cast<LiteNetLib::Utils::INetSerializable*>(this); }
)
