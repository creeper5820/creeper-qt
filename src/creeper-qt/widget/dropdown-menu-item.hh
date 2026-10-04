#pragma once

#include "creeper-qt/utility/qt_wrapper/enter-event.hh"
#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include "creeper-qt/utility/wrapper/widget.hh"

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

    using namespace common::pro;
    using namespace widget::pro;
    using namespace theme::pro;

    using TrailingText =
        common::pro::String<[](auto& self, const auto& string) { self.setTrailingText(string); }>;

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

    struct Measurements {
        DropdownMenuItem::Measurements value;
        explicit Measurements(const DropdownMenuItem::Measurements& value)
            : value { value } { }
        friend auto dsl_invoke(DropdownMenuItem& self, const Measurements& prop) -> void {
            self.setMeasurements(prop.value);
        }
    };

    template <typename F>
    using OnClicked = common::pro::SignalInjection<F, &DropdownMenuItem::clicked>;
}

}
