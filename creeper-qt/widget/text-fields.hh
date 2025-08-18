#pragma once

#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/widget/widget.hh"

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
        void set_color_scheme(const ColorScheme&);

        void load_theme_manager(ThemeManager&);

        void set_label_text(const QString&);

        void set_hint_text(const QString&);

        void set_supporting_text(const QString&);

        void set_leading_icon(const QIcon&);

        void set_leading_icon(const QString& code, const QString& font);

        void set_trailling_icon(const QIcon&);

        void set_trailling_icon(const QString& code, const QString& font);

        struct ColorList {
            QColor container;
            QColor caret;
            QColor active_indicator;

            QColor input_text;
            QColor label_text;
            QColor supporting_text;

            QColor leading_icon;
            QColor trailing_icon;
        };
        struct {
            QColor state_layer;
            QColor selection_container;

            ColorList enabled;
            ColorList disabled;
            ColorList focused;
            ColorList error;
        } color;

    protected:
        void resizeEvent(QResizeEvent*) override;

        void leaveEvent(QEvent*) override;
        void enterEvent(QEvent*) override;

        void focusInEvent(QFocusEvent*) override;
        void focusOutEvent(QFocusEvent*) override;
    };
}
namespace text_field::pro {
    using Token = common::Token<internal::BasicTextField>;

    struct ClearButton : Token {
        bool enabled;
        explicit ClearButton(bool enabled)
            : enabled { enabled } { }
        void apply(auto& self) const { self.setClearButtonEnabled(enabled); }
    };

    struct LabelText : public QString, Token {
        using QString::QString;
        explicit LabelText(const QString& text)
            : QString { text } { }
        void apply(auto& self) const { self.set_label_text(*this); }
    };

    struct LeadingIcon : Token {
        QString code;
        QString font;
        explicit LeadingIcon(const QString& code, const QString& font)
            : code { code }
            , font { font } { }
        void apply(auto& self) const { self.set_leading_icon(code, font); }
    };

    template <class TextField>
    concept trait = std::derived_from<TextField, Token> //
        || widget::pro::trait<TextField>                        //
        || theme::pro::trait<TextField>;

    struct checker {
        template <class T> struct result {
            static constexpr auto v = trait<T>;
        };
    };

    using namespace widget::pro;
    using namespace theme::pro;
}

struct FilledTextField
    : public Declarative<text_field::internal::BasicTextField, text_field::pro::checker> {
    using Declarative::Declarative;
    void paintEvent(QPaintEvent*) override;
};

struct OutlinedTextField
    : public Declarative<text_field::internal::BasicTextField, text_field::pro::checker> {
    using Declarative::Declarative;
    void paintEvent(QPaintEvent*) override;
};

}
