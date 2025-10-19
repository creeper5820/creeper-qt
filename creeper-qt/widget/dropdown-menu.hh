#pragma once

#include "creeper-qt/utility/qt_wrapper/enter-event.hh"
#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include "creeper-qt/utility/wrapper/property.hh"
#include "creeper-qt/utility/wrapper/widget.hh"

#include <qcombobox.h>

namespace creeper {

class FilledDropdownMenu;

namespace dropdown_menu::internal {

    class DropdownMenu : public QComboBox {
        CREEPER_PIMPL_DEFINITION(DropdownMenu);
        friend FilledDropdownMenu;

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

                QColor itemlist_bg;
                QColor item_bg;
                QColor item_selected_bg;
                QColor item_hovered_bg;
            };

            Tokens enabled;
            Tokens disabled;
            Tokens focused;
            Tokens error;

            QColor state_layer;
            QColor selection_container;
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

        void load_theme_manager(ThemeManager&);

        void set_label_text(const QString&);

        void set_leading_icon(const QIcon&);

        void set_leading_icon(const QString& code, const QString& font);

        auto set_measurements(const Measurements&) noexcept -> void;

    protected:
        void resizeEvent(QResizeEvent* event) override;

        void enterEvent(qt::EnterEvent* event) override;
        void leaveEvent(QEvent* event) override;

        void focusInEvent(QFocusEvent*) override;
        void focusOutEvent(QFocusEvent* event) override;

        void changeEvent(QEvent* event) override;

        void showPopup() override;
        void hidePopup() override;

    private:
        friend struct Impl;

    public:
        void setTextMargins(const QMargins& margins);
        QMargins textMargins() const;

    private:
        QMargins margins { 13, 24, 13, 0 };
    };
}

namespace dropdown_menu::pro {

    using Token = common::Token<internal::DropdownMenu>;

    using LabelText = common::pro::String<Token,
        [](auto& self, const auto& string) { self.set_label_text(string); }>;

    struct LeadingIcon : Token {
        QString code;
        QString font;
        explicit LeadingIcon(const QString& code, const QString& font)
            : code { code }
            , font { font } { }
        void apply(auto& self) const { self.set_leading_icon(code, font); }
    };

    template <typename F>
    using IndexChanged =
        common::pro::SignalInjection<F, Token, &internal::DropdownMenu::currentIndexChanged>;

    using Items = DerivedProp<Token, QVector<QString>, //
        [](auto& self, const auto& vec) {
            self.clear();
            self.addItems(vec);
            self.setCurrentIndex(-1);
        }>;

    template <class Select>
    concept trait = std::derived_from<Select, Token>;

    CREEPER_DEFINE_CHECKER(trait);
    using namespace widget::pro;
    using namespace theme::pro;
}

struct FilledDropdownMenu
    : public Declarative<dropdown_menu::internal::DropdownMenu,
          CheckerOr<dropdown_menu::pro::checker, widget::pro::checker, theme::pro::checker>> {
    using Declarative::Declarative;
    void paintEvent(QPaintEvent* event) override;
};
namespace filled_dropdown_menu::pro {
    using namespace dropdown_menu::pro;
}

}
