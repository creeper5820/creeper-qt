#pragma once

#include "creeper-qt/utility/qt_wrapper/enter-event.hh"
#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include "creeper-qt/utility/wrapper/widget.hh"

namespace creeper::dropdown_menu_item::details {

class DropdownMenuItem : public QWidget {
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

    auto set_color_scheme(const ColorScheme&) -> void;

    auto load_theme_manager(ThemeManager&) -> void;

    auto set_measurements(const Measurements&) noexcept -> void;

    auto set_text(const QString&) -> void;

    auto set_leading_icon(const QString& code, const QString& font) -> void;

    auto set_trailing_icon(const QString& code, const QString& font) -> void;

    auto set_trailing_text(const QString&) -> void;

    auto set_disabled(bool) -> void;

    auto set_water_color(const QColor&) -> void;

    auto set_water_ripple_status(bool) -> void;

Q_SIGNALS:
    auto signal_clicked() -> void;

public:
    auto sizeHint() const -> QSize override;

protected:
    auto enterEvent(qt::EnterEvent*) -> void override;
    auto leaveEvent(QEvent*) -> void override;

    auto mousePressEvent(QMouseEvent*) -> void override;
    auto mouseReleaseEvent(QMouseEvent*) -> void override;

    auto paintEvent(QPaintEvent*) -> void override;
};

}

namespace creeper::dropdown_menu_item::pro {

using Token = creeper::Token<details::DropdownMenuItem>;

using Text =
    common::pro::String<Token, [](auto& self, const auto& string) { self.set_text(string); }>;

using TrailingText = common::pro::String<Token,
    [](auto& self, const auto& string) { self.set_trailing_text(string); }>;

struct LeadingIcon : Token {
    QString code;
    QString font;
    explicit LeadingIcon(const QString& code, const QString& font)
        : code { code }
        , font { font } { }
    auto apply(auto& self) const -> void { self.set_leading_icon(code, font); }
};

struct TrailingIcon : Token {
    QString code;
    QString font;
    explicit TrailingIcon(const QString& code, const QString& font)
        : code { code }
        , font { font } { }
    auto apply(auto& self) const -> void { self.set_trailing_icon(code, font); }
};

using Measurements = SetterProp<Token, details::DropdownMenuItem::Measurements,
    [](auto& self, const auto& value) { self.set_measurements(value); }>;

using Disabled = common::pro::Disabled<Token>;

using WaterColor = common::pro::WaterColor<Token>;

template <typename F>
using OnClicked =
    common::pro::SignalInjection<F, Token, &details::DropdownMenuItem::signal_clicked>;

using namespace widget::pro;
using namespace theme::pro;

}

namespace creeper {

using DropdownMenuItem = Declarative<dropdown_menu_item::details::DropdownMenuItem,
    TokenOr<dropdown_menu_item::pro::Token, widget::pro::Token, theme::pro::Token>>;

}
