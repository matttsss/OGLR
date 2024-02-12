#pragma once

#include <cstring>

namespace OGLR {
    template <typename ... VTs>
    struct Vertex
    {
        static constexpr uint32_t N = (sizeof(VTs) + ...);

        char data[N];

        Vertex() = default;
        Vertex(const VTs& ... args) {

            uint32_t idx = 0;
            auto cpyMember = [&](const auto& val) {
                memcpy(data + idx, &val, sizeof(val));
                idx += sizeof(val);
            };

            (cpyMember(args), ...);
        }

        bool operator==(const Vertex<VTs...>& other) const {
            return !memcmp(data, other.data, N);
        }

    };
}
