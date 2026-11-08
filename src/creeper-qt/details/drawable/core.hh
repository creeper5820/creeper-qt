#pragma once

#include <qpainter.h>

namespace creeper::drawable {

namespace details {
    template <class Cpo, class... Args>
    concept dsl_invocable =
        requires(const Cpo& cpo, Args&&... args) { dsl_invoke(cpo, std::forward<Args>(args)...); };

    struct cpo_t {
        template <class Self, class... Args>
            requires dsl_invocable<Self, Args...>
        constexpr auto operator()(this const Self& self, Args&&... args) noexcept(
            noexcept(dsl_invoke(self, std::forward<Args>(args)...))) -> decltype(auto) {
            return dsl_invoke(self, std::forward<Args>(args)...);
        }
    };

    struct draw_t : cpo_t { };
    inline constexpr draw_t draw;

    struct size_t : cpo_t { };
    inline constexpr size_t size;

    template <class U>
    concept DrawUnit = requires(const U& unit, QPainter& handler) {
        { details::draw(unit, handler) };
        { details::size(unit) } -> std::convertible_to<QSizeF>;
    };
}

struct DrawHere {
    template <class... Us>
    explicit DrawHere(QPainter& handler, const Us&... units)
        requires requires { (details::draw(units, handler), ...); }
    {
        (details::draw(units, handler), ...);
    }
};

}
