#pragma once

#include "creeper-qt/details/drawable/core.hh"

namespace creeper::drawable {

template <details::DrawUnit... Us>
struct Box {
    std::tuple<std::decay_t<Us>...> units;

    struct State {
        QSizeF size;
        Qt::Alignment align;
    };
    State state;

    explicit Box(const State& state, const Us&... us) noexcept
        : state { state }
        , units { std::tuple { us... } } { }

    static constexpr auto calculate(const State& state, const QSizeF& size) noexcept {
        const auto h = state.align & Qt::AlignHorizontal_Mask;
        const auto v = state.align & Qt::AlignVertical_Mask;

        auto x = qreal { 0 };
        switch (h) {
        case Qt::AlignLeading:
            x = 0;
            break;
        case Qt::AlignHCenter:
            x = (state.size.width() - size.width()) / 2;
            break;
        case Qt::AlignTrailing:
            x = (state.size.width() - size.width()) / 1;
            break;
        }

        auto y = qreal { 0 };
        switch (v) {
        case Qt::AlignTop:
            y = 0;
            break;
        case Qt::AlignVCenter:
            y = (state.size.height() - size.height()) / 2;
            break;
        case Qt::AlignBottom:
            y = (state.size.height() - size.height()) / 1;
            break;
        }

        return QPointF { x, y };
    }

    friend auto dsl_invoke(details::draw_t, const Box& self, QPainter& handler) {
        handler.save();
        {
            std::apply(
                [&](const auto&... units) {
                    const auto draw_unit = [&](const auto& unit) {
                        handler.save();
                        handler.translate(calculate(self.state, details::size(unit)));
                        details::draw(unit, handler);
                        handler.restore();
                    };
                    (draw_unit(units), ...);
                },
                self.units);
        }
        handler.restore();
    }
    friend auto dsl_invoke(details::size_t, const Box& self) {
        return self.state.size;
    }
};

}
