#pragma once

#include "creeper-qt/utility/api/scope/theme.hh"

#include "creeper-qt/utility/api/helper/signal-injection.hh"
#include "creeper-qt/utility/api/scope/common.hh"

#include "creeper-qt/utility/api/scope/widget.hh"
#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/forward_prop.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"

namespace creeper {

class Slider : public QWidget, public DSL {
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

        constexpr auto minimumHeight() const { return handle_height; }

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

        friend auto dsl_invoke(Slider& self, const Measurements& measurements) -> void {
            self.setMeasurements(measurements);
        }
    };

public:
    explicit Slider(auto&&... props)
        : Slider { } {
        construct_with(std::forward<decltype(props)>(props)...);
    }

    auto loadColorScheme(const ColorScheme&) -> void;
    auto setMeasurements(const Measurements&) -> void;

    auto bindThemeManager(ThemeManager&) -> void;

    auto setProgress(double) noexcept -> void;
    auto getProgress() const noexcept -> double;

Q_SIGNALS:
    auto valueChanged(double) -> void;
    auto valueChangedFinished(double) -> void;

protected:
    auto mousePressEvent(QMouseEvent*) -> void override;
    auto mouseReleaseEvent(QMouseEvent*) -> void override;
    auto mouseMoveEvent(QMouseEvent*) -> void override;

    auto paintEvent(QPaintEvent*) -> void override;
};

namespace slider::pro {
    template <typename F>
    using OnValueChange = api::helper::SignalInjection<F, &Slider::valueChanged>;
    template <typename F>
    using OnValueChangeFinished = api::helper::SignalInjection<F, &Slider::valueChangedFinished>;
    using Progress              = ForwardProp<&Slider::setProgress>;

    using namespace api::scope::common;
    using namespace api::scope::theme;
    using namespace api::scope::widget;
}

}
