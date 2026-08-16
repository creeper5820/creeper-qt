#pragma once

#include "creeper-qt/utility/qt_wrapper/enter-event.hh"
#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include "creeper-qt/utility/wrapper/property.hh"
#include "creeper-qt/utility/wrapper/widget.hh"

#include <qwidget.h>

namespace creeper {

class FilledDropdownMenu;
class OutlinedDropdownMenu;

namespace dropdown_menu::details {

    class DropdownMenu : public QWidget {
        Q_OBJECT
        CREEPER_PIMPL_DEFINITION(DropdownMenu);
        friend FilledDropdownMenu;
        friend OutlinedDropdownMenu;

    public:
        struct ColorSpace {
            struct Tokens {
                QColor container;
                QColor caret;
                QColor active_indicator;

                QColor input_text;
                QColor label_text;
                QColor selected_text;
                QColor supporting_text;

                QColor leading_icon;
                QColor outline;
            };

            Tokens enabled;
            Tokens disabled;
            Tokens focused;
            Tokens error;

            QColor state_layer;
        };

        struct Measurements {
            int container_height = 56;

            int icon_rect_size  = 24;
            int input_rect_size = 24;
            int label_rect_size = 24;

            int standard_font_height = 18;

            int col_padding                      = 8;
            int row_padding_widthout_icons       = 16;
            int row_padding_with_icons           = 12;
            int row_padding_populated_label_text = 4;

            int padding_icons_text = 16;

            int supporting_text_and_character_counter_top_padding = 4;
            int supporting_text_and_character_counter_row_padding = 16;

            auto icon_size() const -> QSize { return QSize { icon_rect_size, icon_rect_size }; };
        };
        auto set_color_scheme(const ColorScheme&) -> void;

        auto load_theme_manager(ThemeManager&) -> void;

        auto set_label_text(const QString&) -> void;

        auto set_leading_icon(const QIcon&) -> void;

        auto set_leading_icon(const QString& code, const QString& font) -> void;

        auto set_measurements(const Measurements&) noexcept -> void;

        auto set_items(const QStringList&) -> void;

        auto set_current_index(int) -> void;
        auto current_index() const noexcept -> int;
        auto current_text() const -> QString;

        auto set_expanded(bool) -> void;
        auto expanded() const noexcept -> bool;

    Q_SIGNALS:
        auto signal_index_changed(int) -> void;

    protected:
        auto enterEvent(qt::EnterEvent* event) -> void override;
        auto leaveEvent(QEvent* event) -> void override;

        auto focusInEvent(QFocusEvent*) -> void override;
        auto focusOutEvent(QFocusEvent* event) -> void override;

        auto mousePressEvent(QMouseEvent*) -> void override;
        auto keyPressEvent(QKeyEvent*) -> void override;
    };
}

namespace dropdown_menu::pro {
    using Token = creeper::Token<details::DropdownMenu>;

    using LabelText = common::pro::String<Token,
        [](auto& self, const auto& string) { self.set_label_text(string); }>;

    struct LeadingIcon : Token {
        QString code;
        QString font;
        explicit LeadingIcon(const QString& code, const QString& font)
            : code { code }
            , font { font } { }
        auto apply(auto& self) const -> void { self.set_leading_icon(code, font); }
    };

    template <typename F>
    using IndexChanged =
        common::pro::SignalInjection<F, Token, &details::DropdownMenu::signal_index_changed>;

    using Items = DerivedProp<Token, QStringList,
        [](auto& self, const auto& items) { self.set_items(items); }>;

    using namespace widget::pro;
    using namespace theme::pro;

    using GroupToken = TokenOr<dropdown_menu::pro::Token, widget::pro::Token, theme::pro::Token>;
}

struct FilledDropdownMenu
    : public Declarative<dropdown_menu::details::DropdownMenu, dropdown_menu::pro::GroupToken> {
    using Declarative::Declarative;
    auto paintEvent(QPaintEvent* event) -> void override;
};
struct OutlinedDropdownMenu
    : public Declarative<dropdown_menu::details::DropdownMenu, dropdown_menu::pro::GroupToken> {
    using Declarative::Declarative;
    auto paintEvent(QPaintEvent* event) -> void override;
};

namespace filled_dropdown_menu::pro {
    using namespace dropdown_menu::pro;
}
namespace outlined_dropdown_menu::pro {
    using namespace dropdown_menu::pro;
}

}
