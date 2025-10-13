#pragma once

#include "creeper-qt/utility/qt_wrapper/enter-event.hh"
#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/widget.hh"

#include <qlineedit.h>

namespace creeper {
class FilledTextField;
class OutlinedTextField;

namespace text_field::internal {
    class BasicTextField : public QLineEdit {
        CREEPER_PIMPL_DEFINITION(BasicTextField);

        friend FilledTextField;
        friend OutlinedTextField;

    public:
        struct ColorSpecs {
            struct Tokens {
                QColor container;
                QColor caret;
                QColor active_indicator;

                QColor input_text;
                QColor label_text;
                QColor supporting_text;

                QColor leading_icon;
                QColor trailing_icon;

                QColor outline;
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
            int label_rect_size = 16;

            int standard_font_height = 16;

            int col_padding                      = 8;
            int row_padding_without_icons        = 16;
            int row_padding_with_icons           = 12;
            int row_padding_populated_label_text = 4;

            int padding_icons_text = 16;

            int supporting_text_and_character_counter_top_padding = 4;
            int supporting_text_and_character_counter_row_padding = 16;

            auto icon_size() const { return QSize { icon_rect_size, icon_rect_size }; }
        };

        void set_color_scheme(const ColorScheme&);

        void load_theme_manager(ThemeManager&);

        void set_label_text(const QString&);

        void set_hint_text(const QString&);

        void set_supporting_text(const QString&);

        void set_leading_icon(const QIcon&);

        void set_leading_icon(const QString& code, const QString& font);

        void set_trailling_icon(const QIcon&);

        void set_trailling_icon(const QString& code, const QString& font);

        auto set_measurements(const Measurements& measurements) noexcept -> void;

    protected:
        void resizeEvent(QResizeEvent*) override;

        void enterEvent(qt::EnterEvent*) override;
        void leaveEvent(QEvent*) override;

        void focusInEvent(QFocusEvent*) override;
        void focusOutEvent(QFocusEvent*) override;
    };
}
namespace text_field::pro {
    using Token = common::Token<internal::BasicTextField>;

    using ClearButton = SetterProp<Token, bool,
        [](auto& self, bool enable) { self.setClearButtonEnabled(enable); }>;

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
    using OnTextChanged =
        common::pro::SignalInjection<F, Token, &internal::BasicTextField::textChanged>;

    template <typename F>
    using OnEditingFinished =
        common::pro::SignalInjection<F, Token, &internal::BasicTextField::editingFinished>;

    template <typename F>
    using OnChanged = OnTextChanged<F>;

    template <class TextField>
    concept trait = std::derived_from<TextField, Token>;

    CREEPER_DEFINE_CHECKER(trait);
    using namespace widget::pro;
    using namespace theme::pro;
}

struct FilledTextField
    : public Declarative<text_field::internal::BasicTextField,
          CheckerOr<text_field::pro::checker, widget::pro::checker, theme::pro::checker>> {
    using Declarative::Declarative;
    void paintEvent(QPaintEvent*) override;
};

struct OutlinedTextField
    : public Declarative<text_field::internal::BasicTextField,
          CheckerOr<text_field::pro::checker, widget::pro::checker, theme::pro::checker>> {
    using Declarative::Declarative;
    void paintEvent(QPaintEvent*) override;
};

}
