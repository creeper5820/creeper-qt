#pragma once

#include "creeper-qt/utility/api/scope/common.hh"

#include <qabstractbutton.h>
#include <qpainter.h>

#include "creeper-qt/utility/qt_wrapper/enter-event.hh"
#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/forward_prop.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include "creeper-qt/utility/wrapper/widget.hh"

namespace creeper {

class IconButton : public QAbstractButton, public DSL {
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
    explicit IconButton(auto&&... props)
        : IconButton { } {
        construct_with(std::forward<decltype(props)>(props)...);
    }

    auto loadColorScheme(const ColorScheme&) noexcept -> void;
    auto bindThemeManager(ThemeManager&) noexcept -> void;

    auto setFontIcon(const QString&) noexcept -> void;
    auto setIcon(const QIcon&) noexcept -> void;

    auto setTypes(Types) noexcept -> void;
    auto setShape(Shape) noexcept -> void;
    auto setColor(Color) noexcept -> void;
    auto setWidth(Width) noexcept -> void;

    auto typesEnum() const noexcept -> Types;
    auto shapeEnum() const noexcept -> Shape;
    auto colorEnum() const noexcept -> Color;
    auto widthEnum() const noexcept -> Width;

    auto selected() const noexcept -> bool;
    auto setSelected(bool) noexcept -> void;

    // TODO: 详细的颜色自定义接口有缘再写

protected:
    auto resizeEvent(QResizeEvent*) -> void override;
    auto enterEvent(qt::EnterEvent*) -> void override;
    auto leaveEvent(QEvent*) -> void override;

    auto paintEvent(QPaintEvent*) -> void override;
};

namespace icon_button::pro {

    using namespace common::pro;
    using namespace api::scope::common;
    using namespace widget::pro;
    using namespace theme::pro;

    using Icon     = ForwardProp<&IconButton::setIcon>;
    using FontIcon = ForwardProp<&IconButton::setFontIcon>;

    using Color = ForwardProp<&IconButton::setColor>;
    using Shape = ForwardProp<&IconButton::setShape>;
    using Types = ForwardProp<&IconButton::setTypes>;
    using Width = ForwardProp<&IconButton::setWidth>;

    constexpr auto ColorFilled   = Color { IconButton::Color::DEFAULT_FILLED };
    constexpr auto ColorOutlined = Color { IconButton::Color::OUTLINED };
    constexpr auto ColorStandard = Color { IconButton::Color::STANDARD };
    constexpr auto ColorTonal    = Color { IconButton::Color::TONAL };

    constexpr auto ShapeRound  = Shape { IconButton::Shape::DEFAULT_ROUND };
    constexpr auto ShapeSquare = Shape { IconButton::Shape::SQUARE };

    constexpr auto TypesDefault          = Types { IconButton::Types::DEFAULT };
    constexpr auto TypesToggleSelected   = Types { IconButton::Types::TOGGLE_SELECTED };
    constexpr auto TypesToggleUnselected = Types { IconButton::Types::TOGGLE_UNSELECTED };

    constexpr auto WidthDefault = Width { IconButton::Width::DEFAULT };
    constexpr auto WidthNarrow  = Width { IconButton::Width::NARROW };
    constexpr auto WidthWide    = Width { IconButton::Width::WIDE };

    template <typename Callback>
    using Clickable = common::pro::Clickable<Callback>;

} // namespace icon_button::pro

} // namespace creeper
