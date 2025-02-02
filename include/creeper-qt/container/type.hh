#pragma once

#include <cstdint>

namespace creeper {

enum class Orientation : uint8_t {
    Vertical,
    Horizon,
};

struct Alignment {
    enum class Vertical : uint8_t { Top, Bottom, Center, Fill };
    enum class Horizon : uint8_t { Start, End, Center, Fill };

    explicit Alignment(Vertical vertical)
        : vertical_(vertical) { }
    explicit Alignment(Horizon horizon)
        : horizon_(horizon) { }
    explicit Alignment(Vertical vertical, Horizon horizon)
        : vertical_(vertical)
        , horizon_(horizon) { }

    Vertical vertical() const { return vertical_; }
    Horizon horizon() const { return horizon_; };

private:
    Vertical vertical_ = Vertical::Fill;
    Horizon horizon_ = Horizon::Fill;
};

}