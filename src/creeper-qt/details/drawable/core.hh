#pragma once

#include "creeper-qt/details/dsl/core.hh"

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
}

struct DrawHere {
    template <class... Us>
    explicit DrawHere(QPainter& handler, const Us&... units)
        requires requires { (details::draw(units, handler), ...); }
    {
        (details::draw(units, handler), ...);
    }
};

struct Line {
    QPoint point;
    QColor color;
    double width;

    friend auto dsl_invoke(details::draw_t, const Line& line, QPainter& handler) {
        handler.save();
        {
            std::ignore = line.point;
            std::ignore = line.color;
        }
        handler.restore();
    }
    friend auto dsl_invoke(details::size_t, const Line& line) {
        return QSize {
            std::abs(line.point.x()),
            std::abs(line.point.y()),
        };
    }
};

template <class... Us>
struct Box {
    std::tuple<std::decay_t<Us>...> units;

    struct State {
        QSize size;
        Qt::Alignment align;
    };
    State state;

    explicit Box(const State& state, const Us&... us) noexcept
        : state { state }
        , units { std::tuple { us... } } { }

    static constexpr auto calculate(const State& state, const QSize& size) noexcept {
        const auto h = state.align & Qt::AlignHorizontal_Mask;
        const auto v = state.align & Qt::AlignVertical_Mask;

        auto x = int { 0 };
        switch (h) {
        case Qt::AlignLeading:
            x = 0;
            break;
        case Qt::AlignHCenter:
            x = (state.size.width() - size.width()) / 2;
            break;
        case Qt::AlignTrailing:
            x = (state.size.width() - size.width());
            break;
        }

        auto y = int { 0 };
        switch (v) {
        case Qt::AlignTop:
            y = 0;
            break;
        case Qt::AlignVCenter:
            y = (state.size.height() - size.height()) / 2;
            break;
        case Qt::AlignBottom:
            y = (state.size.height() - size.height());
            break;
        }

        return QPoint { x, y };
    }

    friend auto dsl_invoke(details::draw_t, const Box& line, QPainter& handler) {
        handler.save();
        {
            // ...
        }
        handler.restore();
    }
    friend auto dsl_invoke(details::size_t, const Box& area) {
        return area.state.size;
    }
};

template <class U>
struct If {
    std::decay_t<U> unit;
    bool flag;

    explicit If(bool flag, const U& unit)
        : flag { flag }
        , unit { unit } { }

    friend auto dsl_invoke(details::draw_t, const If& self, QPainter& handler) {
        if (self.flag) {
            handler.save();
            {
                // ...
            }
            handler.restore();
        }
    }
    friend auto dsl_invoke(details::size_t, const If& self) {
        return self.flag ? details::size(self.unit) : QSizeF {};
    }
};

}

struct DraftClass {
    auto paint_event(QPaintDevice* device) {
        using namespace creeper::drawable;

        bool oneline = false;

        auto handler = QPainter { device };
        DrawHere {
            handler,
            Box {
              {
                .size  = {},
                .align = Qt::AlignCenter,
              },
              Line {
                .point = { 5, 0 },
                .color = Qt::black,
              },
              If {
                !oneline,
                Line {
                  .point = { 0, 5 },
                  .color = Qt::black,
                },
              },
            },
        };
    }
};
