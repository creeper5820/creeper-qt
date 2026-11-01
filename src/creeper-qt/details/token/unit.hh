#pragma once

#include <compare> // IWYU pragma: keep

namespace creeper {

struct Dp {
    double value;

    constexpr auto px() const -> int { return static_cast<int>(value); }

    constexpr auto operator<=>(const Dp&) const = default;
    constexpr auto operator+(Dp o) const -> Dp { return { value + o.value }; }
    constexpr auto operator-(Dp o) const -> Dp { return { value - o.value }; }
    constexpr auto operator*(double k) const -> Dp { return { value * k }; }
    constexpr auto operator/(double k) const -> Dp { return { value / k }; }
    constexpr auto operator/(Dp o) const -> double { return value / o.value; }
};

struct Sp {
    double value;

    constexpr auto px() const -> int { return static_cast<int>(value); }

    constexpr auto operator<=>(const Sp&) const = default;
    constexpr auto operator+(Sp o) const -> Sp { return { value + o.value }; }
    constexpr auto operator-(Sp o) const -> Sp { return { value - o.value }; }
    constexpr auto operator*(double k) const -> Sp { return { value * k }; }
    constexpr auto operator/(double k) const -> Sp { return { value / k }; }
    constexpr auto operator/(Sp o) const -> double { return value / o.value; }
};

constexpr auto operator*(double k, Dp o) -> Dp { return o * k; }
constexpr auto operator*(double k, Sp o) -> Sp { return o * k; }

inline namespace literals {
    consteval auto operator""_dp(unsigned long long v) -> Dp { return { static_cast<double>(v) }; }
    consteval auto operator""_dp(long double v) -> Dp { return { static_cast<double>(v) }; }
    consteval auto operator""_sp(unsigned long long v) -> Sp { return { static_cast<double>(v) }; }
    consteval auto operator""_sp(long double v) -> Sp { return { static_cast<double>(v) }; }
}

}
