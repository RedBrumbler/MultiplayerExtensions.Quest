#include "Players/MpexPlayerData.hpp"
#include "config.hpp"
#include "logging.hpp"

#include "bsml/shared/StringParseHelper.hpp"
#include <sstream>

DEFINE_TYPE(MultiplayerExtensions::Players, MpexPlayerData);

inline uint8_t to_byte(float f) { return static_cast<uint8_t>(std::clamp(f * 256, 0.0f, 256.0f)); }

namespace MultiplayerExtensions::Players {
    void MpexPlayerData::ctor() {}
    void MpexPlayerData::Serialize(LiteNetLib::Utils::NetDataWriter* writer) {
        // TODO: check if this makes a correct html color
        StringW html(fmt::format("#{:2X}{:2X}{:2X}", to_byte(playerColor.r), to_byte(playerColor.g), to_byte(playerColor.b)));
        writer->Put(html);
    }

    void MpexPlayerData::Deserialize(LiteNetLib::Utils::NetDataReader* reader) {
        std::string html(reader->GetString());

        playerColor = StringParseHelper(html).tryParseColor().value_or(UnityEngine::Color(0.031f, 0.752f, 1.0f, 1.0f));
    }
}
