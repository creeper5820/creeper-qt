#pragma once

#include <qabstractbutton.h>
#include <qpainter.h>

#include "creeper-qt/utility/qt_wrapper/enter_event.hh"
#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include "creeper-qt/utility/wrapper/property.hh"
#include "creeper-qt/utility/wrapper/widget.hh"

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
    auto enterEvent(qt::EnterEvent*) -> void override;
    auto leaveEvent(QEvent*) -> void override;

    auto paintEvent(QPaintEvent*) -> void override;
};
}
namespace creeper::icon_button::pro {
using Token = common::Token<internal::IconButton>;

using Icon =
    creeper::DerivedProp<Token, QIcon, [](auto& self, const auto& v) { self.set_icon(v); }>;
using FontIcon =
    creeper::DerivedProp<Token, QString, [](auto& self, const auto& v) { self.set_icon(v); }>;

using Color = creeper::SetterProp<Token, internal::IconButton::Color,
    [](auto& self, const auto& v) { self.set_color(v); }>;
using Shape = creeper::SetterProp<Token, internal::IconButton::Shape,
    [](auto& self, const auto& v) { self.set_shape(v); }>;
using Types = creeper::SetterProp<Token, internal::IconButton::Types,
    [](auto& self, const auto& v) { self.set_types(v); }>;
using Width = creeper::SetterProp<Token, internal::IconButton::Width,
    [](auto& self, const auto& v) { self.set_width(v); }>;

constexpr auto ColorFilled   = Color { internal::IconButton::Color::DEFAULT_FILLED };
constexpr auto ColorOutlined = Color { internal::IconButton::Color::OUTLINED };
constexpr auto ColorStandard = Color { internal::IconButton::Color::STANDARD };
constexpr auto ColorTonal    = Color { internal::IconButton::Color::TONAL };

constexpr auto ShapeRound  = Shape { internal::IconButton::Shape::DEFAULT_ROUND };
constexpr auto ShapeSquare = Shape { internal::IconButton::Shape::SQUARE };

constexpr auto TypesDefault          = Types { internal::IconButton::Types::DEFAULT };
constexpr auto TypesToggleSelected   = Types { internal::IconButton::Types::TOGGLE_SELECTED };
constexpr auto TypesToggleUnselected = Types { internal::IconButton::Types::TOGGLE_UNSELECTED };

constexpr auto WidthDefault = Width { internal::IconButton::Width::DEFAULT };
constexpr auto WidthNarrow  = Width { internal::IconButton::Width::NARROW };
constexpr auto WidthWide    = Width { internal::IconButton::Width::WIDE };

template <typename Callback>
using Clickable = common::pro::Clickable<Callback, Token>;

template <class T>
concept trait = std::derived_from<T, Token>;

CREEPER_DEFINE_CHECKER(trait);

using namespace widget::pro;
using namespace theme::pro;
}
namespace creeper {

using IconButton = Declarative<icon_button::internal::IconButton,
    CheckerOr<icon_button::pro::checker, widget::pro::checker, theme::pro::checker>>;

}
