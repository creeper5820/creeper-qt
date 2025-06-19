#pragma once

#include <qabstractbutton.h>

#include "utility/theme/theme.hh"
#include "utility/wrapper/pimpl.hh"
#include "utility/wrapper/property.hh"

namespace creeper {
namespace icon_button::internal {
    class IconButton : public QAbstractButton {
        CREEPER_PIMPL_DEFINTION(IconButton);

    public:
        enum class Shape { DEFAULT_ROUND, SQUARE };
        enum class Color { DEFAULT_FILLED, TONAL, OUTLINED, STANDARD };
        enum class Width { DEFAULT, NARROW, WIDE };

    public:
        void set_color_scheme(const ColorScheme& pack);
        void load_theme_manager(ThemeManager& manager);
    };
}
namespace icon_button::pro {
    using Token = common::Token<internal::IconButton>;

    template <class T>
    concept property_concept = std::derived_from<T, Token> //
        || util::theme::pro::property_concept<T>;

    using namespace util::theme::pro;
}
class IconButton : public icon_button::internal::IconButton {
    CREEPER_DEFINE_CONSTROCTOR(IconButton, icon_button::pro);
};
}
