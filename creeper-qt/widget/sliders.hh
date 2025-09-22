#pragma once

#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include "creeper-qt/widget/widget.hh"

namespace creeper::slider::internal {

class Slider : public Widget {
    Q_OBJECT
    CREEPER_PIMPL_DEFINITION(Slider)

public:
    struct ColorSpecs {
        struct Tokens {
            QColor value_indicator = Qt::black;
            QColor value_text      = Qt::white;

            QColor stop_indicator_active   = Qt::white;
            QColor stop_indicator_inactive = Qt::black;

            QColor track_active   = Qt::black;
            QColor track_inactive = Qt::gray;

            QColor handle = Qt::black;
        };
        Tokens enabled;
        Tokens disabled;
    };

    struct Measurements {
        int track_height = 16;

        int label_container_height = 44;
        int label_container_width  = 48;

        int handle_height = 44;
        int handle_width  = 4;

        int track_shape = 8;

        int inset_icon_size = 0;

        static constexpr auto Xs() {
            return Measurements {
                .track_height    = 16,
                .handle_height   = 44,
                .track_shape     = 8,
                .inset_icon_size = 0,
            };
        }
        static constexpr auto S() {
            return Measurements {
                .track_height    = 24,
                .handle_height   = 44,
                .track_shape     = 8,
                .inset_icon_size = 0,
            };
        }
        static constexpr auto M() {
            return Measurements {
                .track_height    = 40,
                .handle_height   = 52,
                .track_shape     = 12,
                .inset_icon_size = 24,
            };
        }
        static constexpr auto L() {
            return Measurements {
                .track_height    = 56,
                .handle_height   = 68,
                .track_shape     = 16,
                .inset_icon_size = 24,
            };
        }
        static constexpr auto SL() {
            return Measurements {
                .track_height    = 96,
                .handle_height   = 108,
                .track_shape     = 28,
                .inset_icon_size = 32,
            };
        }
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
    F f;
    explicit OnValueChange(F f) noexcept
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
