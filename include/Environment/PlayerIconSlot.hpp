#pragma once

#include "beatsaber-hook/shared/utils/il2cpp-type-check.hpp"

namespace MultiplayerExtensions::Environment {
    enum PlayerIconSlot {
        Platform
    };
}

template<>
struct ::il2cpp_utils::il2cpp_type_check::il2cpp_no_arg_class<MultiplayerExtensions::Environment::PlayerIconSlot> {
    static inline Il2CppClass* get() {
        return classof(int);
    }
};
static_assert(sizeof(MultiplayerExtensions::Environment::PlayerIconSlot) == sizeof(int));

#include <fmt/core.h>

template <> struct fmt::formatter<::MultiplayerExtensions::Environment::PlayerIconSlot> : formatter<string_view> {
    // parse is inherited from formatter<string_view>.
    template <typename FormatContext>
    auto format(::MultiplayerExtensions::Environment::PlayerIconSlot c, FormatContext& ctx) {
        switch (c) {
            case MultiplayerExtensions::Environment::PlayerIconSlot::Platform:
                return formatter<string_view>::format("Platform", ctx);;
        }
    }
};
