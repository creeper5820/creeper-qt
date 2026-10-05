#pragma once

#include "creeper-qt/utility/api/helper/signal-injection.hh" // IWYU pragma: keep
#include "creeper-qt/utility/api/helper/string.hh"           // IWYU pragma: keep
#include "creeper-qt/utility/api/pro/disabled.hh"            // IWYU pragma: keep
#include "creeper-qt/utility/api/pro/text.hh"                // IWYU pragma: keep
#include "creeper-qt/utility/api/pro/water-color.hh"         // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/common.hh"            // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/theme.hh"             // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/widget.hh"            // IWYU pragma: keep
#include "creeper-qt/utility/qt_wrapper/enter-event.hh"
#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"

namespace creeper {

class DropdownMenuItem : public QWidget, public DSL {
    Q_OBJECT
    CREEPER_PIMPL_DEFINITION(DropdownMenuItem)

public:
    struct ColorSpace {
        struct Tokens {
            QColor label_text;
            QColor leading_icon;
            QColor trailing_icon;
        };

        Tokens enabled;
        Tokens disabled;

        QColor hover_state_layer;
        QColor pressed_state_layer;
    };

    struct Measurements {
        int height             = 48;
        int horizontal_padding = 12;
        int icon_size          = 24;
        int icon_text_spacing  = 12;
        int label_font_size    = 14;

        friend auto dsl_invoke(DropdownMenuItem& self, const Measurements& measurements) -> void {
            self.setMeasurements(measurements);
        }
    };

    explicit DropdownMenuItem(auto&&... props)
        : DropdownMenuItem { } {
        construct_with(std::forward<decltype(props)>(props)...);
    }

    auto loadColorScheme(const ColorScheme&) -> void;

    auto bindThemeManager(ThemeManager&) -> void;

    auto setMeasurements(const Measurements&) noexcept -> void;

    auto setText(const QString&) -> void;

    auto setLeadingIcon(const QString& code, const QString& font) -> void;

    auto setTrailingIcon(const QString& code, const QString& font) -> void;

    auto setTrailingText(const QString&) -> void;

    auto setDisabled(bool) -> void;

    auto setWaterColor(const QColor&) -> void;

    auto setWaterRippleStatus(bool) -> void;

Q_SIGNALS:
    auto clicked() -> void;

public:
    auto sizeHint() const -> QSize override;

protected:
    auto enterEvent(qt::EnterEvent*) -> void override;
    auto leaveEvent(QEvent*) -> void override;

    auto mousePressEvent(QMouseEvent*) -> void override;
    auto mouseReleaseEvent(QMouseEvent*) -> void override;

    auto paintEvent(QPaintEvent*) -> void override;
};

namespace dropdown_menu_item::pro {
    using TrailingText =
        api::helper::String<[](auto& self, const auto& string) { self.setTrailingText(string); }>;
    struct LeadingIcon {
        QString code;
        QString font;
        explicit LeadingIcon(const QString& code, const QString& font)
            : code { code }
            , font { font } { }
        friend auto dsl_invoke(DropdownMenuItem& self, const LeadingIcon& prop) -> void {
            self.setLeadingIcon(prop.code, prop.font);
        }
    };
    struct TrailingIcon {
        QString code;
        QString font;
        explicit TrailingIcon(const QString& code, const QString& font)
            : code { code }
            , font { font } { }
        friend auto dsl_invoke(DropdownMenuItem& self, const TrailingIcon& prop) -> void {
            self.setTrailingIcon(prop.code, prop.font);
        }
    };
    template <typename F>
    using OnClicked = api::helper::SignalInjection<F, &DropdownMenuItem::clicked>;

    using api::pro::Disabled;
    using api::pro::Text;
    using api::pro::WaterColor;

    using namespace api::scope::common;
    using namespace api::scope::theme;
    using namespace api::scope::widget;
}

}
