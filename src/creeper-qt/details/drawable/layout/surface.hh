#pragma once

#include "creeper-qt/details/drawable/core.hh"

namespace creeper::drawable {

template <details::DrawUnit... Us>
struct Surface {
    std::tuple<std::decay_t<Us>...> units;

    struct State {
        QSizeF size;
        QPointF origin;
    };
    State state;

    explicit Surface(const State& state, const Us&... us) noexcept
        : state { state }
        , units { std::tuple { us... } } { }

    friend auto dsl_invoke(details::draw_t, const Surface& self, QPainter& handler) {
        handler.save();
        {
            handler.translate(self.state.origin);
            auto draw = [&](const auto&... units) { //
                (details::draw(units, handler), ...);
            };
            std::apply(draw, self.units);
        }
        handler.restore();
    }

    friend auto dsl_invoke(details::size_t, const Surface& self) {
        return self.state.size;
    }
};

}
