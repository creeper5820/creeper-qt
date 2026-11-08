#pragma once

#include "creeper-qt/details/drawable/core.hh"

namespace creeper::drawable {

template <details::DrawUnit U>
struct If {
    std::decay_t<U> unit;
    bool flag;

    explicit If(bool flag, const U& unit)
        : flag { flag }
        , unit { unit } { }

    friend auto dsl_invoke(details::draw_t, const If& self, QPainter& handler) {
        if (self.flag) {
            handler.save();
            details::draw(self.unit, handler);
            handler.restore();
        }
    }
    friend auto dsl_invoke(details::size_t, const If& self) -> QSizeF {
        return self.flag ? QSizeF { details::size(self.unit) } : QSizeF {};
    }
};

}
