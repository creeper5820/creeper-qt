#pragma once
#include <concepts>

namespace creeper::animate {

template <typename T>
struct numeric_limits;

template <std::integral T>
struct numeric_limits<T> {
    static constexpr T zero    = T { 0 };
    static constexpr T epsilon = T { 5 };
};

template <std::floating_point T>
struct numeric_limits<T> {
    static constexpr T zero    = T { 0.0 };
    static constexpr T epsilon = T { 1e-3 };
};

}
