#pragma once

#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include "creeper-qt/utility/wrapper/widget.hh"

namespace creeper::slider::internal {

class Slider : public Widget {
    Q_OBJECT
    CREEPER_PIMPL_DEFINITION(Slider)

public:
    struct ColorSpecs {
        struct Tokens {
            QColor stop_indicator;
            QColor container;
            QColor track;
            QColor handle;
        };
        Tokens enabled;
        Tokens disabled;
    };

signals:
    auto signal_value_change(double) -> void;
    auto signal_value_change_finished(double) -> void;

protected:
    auto mousePressEvent(QMouseEvent*) -> void override;
    auto mouseReleaseEvent(QMouseEvent*) -> void override;
    auto mouseMoveEvent(QMouseEvent*) -> void override;

    auto paintEvent(QPaintEvent*) -> void override;
};

}
namespace creeper::slider::pro {

using Token = common::Token<internal::Slider>;

template <typename F>
struct OnValueChange : Token {
    std::decay_t<F> f;
    explicit OnValueChange(F&& f) noexcept
        requires std::invocable<F, double>
        : f { std::forward<decltype(f)>(f) } { }
    auto apply(auto& self) const noexcept -> void {
        QObject::connect(&self, &internal::Slider::signal_value_change, f);
    }
};

template <class T>
concept trait = std::derived_from<T, Token>;

CREEPER_DEFINE_CHECKER(trait)
using namespace widget::pro;
using namespace theme::pro;
}
namespace creeper {

using Slider = Declarative<slider::internal::Slider,
    CheckerOr<slider::pro::checker, widget::pro::checker, theme::pro::checker>>;

}
