#pragma once

#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/common-property.hh"
#include "creeper-qt/widget/widget.hh"

#include <qlineedit.h>

namespace creeper {
namespace text_field::internal {
    class BasicTextField : public QLineEdit {
        CREEPER_PIMPL_DEFINTION(BasicTextField);

    public:
        void set_color_scheme(const ColorScheme&);
        void load_theme_manager(ThemeManager&);

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
            ColorList current;
        } color;

    protected:
        void enterEvent(QEvent*) override;
        void leaveEvent(QEvent*) override;
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

    template <class TextField>
    concept property_concept = std::derived_from<TextField, Token> //
        || widget::pro::property_concept<TextField>                //
        || util::theme::pro::property_concept<TextField>;

    using namespace widget::pro;
    using namespace util::theme::pro;
}

class FilledTextField : public text_field::internal::BasicTextField {
    CREEPER_DEFINE_CONSTROCTOR(FilledTextField, text_field::pro);
    using text_field::internal::BasicTextField::BasicTextField;

protected:
    void paintEvent(QPaintEvent*) override;
};

class OutlinedTextField : public text_field::internal::BasicTextField {
    CREEPER_DEFINE_CONSTROCTOR(OutlinedTextField, text_field::pro);
    using text_field::internal::BasicTextField::BasicTextField;

protected:
    void paintEvent(QPaintEvent*) override;
};

}
