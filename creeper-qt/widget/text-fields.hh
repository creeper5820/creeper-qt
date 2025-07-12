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

            QColor input_text;
            QColor label_text;
            QColor supporting_text;

            QColor leading_icon;
            QColor trailing_icon;
        };

        ColorList enabled_color_list;
        ColorList disabled_color_list;
        ColorList hovered_color_list;
        ColorList focused_color_list;
        ColorList error_color_list;

        ColorList current_color_list;

    protected:
        void enterEvent(QEvent*) override;
        void leaveEvent(QEvent*) override;

        void paintEvent(QPaintEvent*) override;
    };
}
namespace text_field::pro {
    using Token = common::Token<internal::BasicTextField>;

    struct ContainerColor : Token {
        QColor color;
        explicit ContainerColor(const QColor& color)
            : color { color } { }
        void apply(auto& self) const { self.current_color_list.container = color; }
    };

    template <class TextField>
    concept property_concept = std::derived_from<TextField, Token> //
        || widget::pro::property_concept<TextField>                //
        || util::theme::pro::property_concept<TextField>;
}
class FilledTextField : public text_field::internal::BasicTextField {
    CREEPER_DEFINE_CONSTROCTOR(FilledTextField, text_field::pro);
    using text_field::internal::BasicTextField::BasicTextField;
};
}
