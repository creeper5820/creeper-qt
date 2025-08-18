#pragma once

#include <qabstractbutton.h>
#include <qpainter.h>

#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include "creeper-qt/utility/wrapper/property.hh"

#include "creeper-qt/widget/widget.hh"

namespace creeper::icon_button::internal {
class IconButton : public QAbstractButton {
    CREEPER_PIMPL_DEFINITION(IconButton);

public:
    enum class Types { DEFAULT, TOGGLE_UNSELECTED, TOGGLE_SELECTED };

    enum class Shape { DEFAULT_ROUND, SQUARE };

    enum class Color { DEFAULT_FILLED, TONAL, OUTLINED, STANDARD };

    enum class Width { DEFAULT, NARROW, WIDE };

    /// @brief
    ///     依照文档 https://m3.material.io/components/icon-buttons/specs
    ///     给出如下标准容器尺寸，图标尺寸和字体大小
    /// @note
    ///     该组件支持 Material Symbols，只要安装相关字体即可使用，下面的
    ///     FontIcon Size 也是根据字体的大小而定的, utility/material-icon.hh
    ///     文件中有一些预定义的字体和图标编码

    // Extra Small
    static constexpr auto kExtraSmallContainerSize = QSize { 32, 32 };
    static constexpr auto kExtraSmallIconSize      = QSize { 20, 20 };
    static constexpr auto kExtraSmallFontIconSize  = int { 15 };
    // Small
    static constexpr auto kSmallContainerSize = QSize { 40, 40 };
    static constexpr auto kSmallIconSize      = QSize { 24, 24 };
    static constexpr auto kSmallFontIconSize  = int { 18 };
    // Medium
    static constexpr auto kMediumContainerSize = QSize { 56, 56 };
    static constexpr auto kMediumIconSize      = QSize { 24, 24 };
    static constexpr auto kMediumFontIconSize  = int { 18 };
    // Large
    static constexpr auto kLargeContainerSize = QSize { 96, 96 };
    static constexpr auto kLargeIconSize      = QSize { 32, 32 };
    static constexpr auto kLargeFontIconSize  = int { 24 };
    // Extra Large
    static constexpr auto kExtraLargeContainerSize = QSize { 136, 136 };
    static constexpr auto kExtraLargeIconSize      = QSize { 40, 40 };
    static constexpr auto kExtraLargeFontIconSize  = int { 32 };

public:
    auto set_color_scheme(const ColorScheme&) noexcept -> void;
    auto load_theme_manager(ThemeManager&) noexcept -> void;

    auto set_icon(const QString&) noexcept -> void;
    auto set_icon(const QIcon&) noexcept -> void;

    auto set_types(Types) noexcept -> void;
    auto set_shape(Shape) noexcept -> void;
    auto set_color(Color) noexcept -> void;
    auto set_width(Width) noexcept -> void;

    auto types_enum() const noexcept -> Types;
    auto shape_enum() const noexcept -> Shape;
    auto color_enum() const noexcept -> Color;
    auto width_enum() const noexcept -> Width;

    auto selected() const noexcept -> bool;
    auto set_selected(bool) noexcept -> void;

    // TODO: 详细的颜色自定义接口有缘再写

protected:
    auto enterEvent(QEvent*) -> void override;
    auto leaveEvent(QEvent*) -> void override;

    auto paintEvent(QPaintEvent*) -> void override;
};
}
namespace creeper::icon_button::pro {
using Token = common::Token<internal::IconButton>;

struct Icon : public QIcon, Token {
    using QIcon::QIcon;
    explicit Icon(const QIcon& icon)
        : QIcon { icon } { }
    void apply(auto& self) const { self.set_icon(*this); }
};
struct FontIcon : public QString, Token {
    using QString::QString;
    explicit FontIcon(const QString& icon)
        : QString { icon } { }
    void apply(auto& self) const { self.set_icon(*this); }
};

struct Color : Token {
    using Enum = internal::IconButton::Color;
    Enum color;
    explicit Color(Enum p) { color = p; }
    void apply(auto& self) const { self.set_color(color); }
};
namespace color {
    inline const auto DEFAULT_FILLED = Color { Color::Enum::DEFAULT_FILLED };
    inline const auto OUTLINED       = Color { Color::Enum::OUTLINED };
    inline const auto STANDARD       = Color { Color::Enum::STANDARD };
    inline const auto TONAL          = Color { Color::Enum::TONAL };
}

struct Shape : Token {
    using Enum = internal::IconButton::Shape;
    Enum shape;

    explicit Shape(Enum p)
        : shape { p } { }

    void apply(auto& self) const { self.set_shape(shape); }
};
namespace shape {
    inline const auto DEFAULT_ROUND = Shape { Shape::Enum::DEFAULT_ROUND };
    inline const auto SQUARE        = Shape { Shape::Enum::SQUARE };
}

struct Types : Token {
    using Enum = internal::IconButton::Types;
    Enum types;

    explicit Types(Enum p)
        : types { p } { }

    void apply(auto& self) const { self.set_types(types); }
};
namespace types {
    inline const auto DEFAULT           = Types { Types::Enum::DEFAULT };
    inline const auto TOGGLE_SELECTED   = Types { Types::Enum::TOGGLE_SELECTED };
    inline const auto TOGGLE_UNSELECTED = Types { Types::Enum::TOGGLE_UNSELECTED };
}

struct Width : Token {
    using Enum = internal::IconButton::Width;
    Enum width;

    explicit Width(Enum p)
        : width { p } { }

    void apply(auto& self) const { self.set_width(width); }
};
namespace width {
    inline const auto DEFAULT = Width { Width::Enum::DEFAULT };
    inline const auto NARROW  = Width { Width::Enum::NARROW };
    inline const auto WIDE    = Width { Width::Enum::WIDE };
}

template <typename Callback> using Clickable = common::pro::Clickable<Callback, Token>;

template <class T>
concept trait = std::derived_from<T, Token> //
    || theme::pro::trait<T>                 //
    || widget::pro::trait<T>;

CREEPER_DEFINE_CHECK(trait);

using namespace theme::pro;
using namespace widget::pro;
}
namespace creeper {

using IconButton = Declarative<icon_button::internal::IconButton, icon_button::pro::checker>;

}
