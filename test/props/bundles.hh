#pragma once

#include <creeper-qt/creeper-qt.hh>

#include <tuple>

namespace creeper::test {

/// api::scope::widget 的全部属性（适用于任意 QWidget 派生的 creeper 组件）
inline const auto kWidgetProps = std::tuple {
    api::pro::BackgroundRole { QPalette::Window },
    api::pro::BaseSize { 10, 10 },
    api::pro::BitmapMask { QBitmap { 10, 10 } },
    api::pro::ClearMask { },
    api::pro::FixedHeight { 10 },
    api::pro::FixedSize { 10, 10 },
    api::pro::FixedWidth { 10 },
    api::pro::Font { QFont { } },
    api::pro::ForegroundRole { QPalette::WindowText },
    api::pro::GraphicsEffect { nullptr },
    api::pro::Layout<Row> { },
    api::pro::LayoutDirection { Qt::LeftToRight },
    api::pro::MaximumHeight { 100 },
    api::pro::MaximumSize { 100, 100 },
    api::pro::MaximumWidth { 100 },
    api::pro::MinimumHeight { 10 },
    api::pro::MinimumSize { 10, 10 },
    api::pro::MinimumWidth { 10 },
    api::pro::MoveCenter { },
    api::pro::Parent { nullptr },
    api::pro::RegionMask { },
    api::pro::SizeIncrement { 1, 1 },
    api::pro::SizePolicy { QSizePolicy::Preferred },
    api::pro::ToolTip { QString { } },
    api::pro::WindowFilePath { QString { } },
    api::pro::WindowFlag { Qt::Widget },
    api::pro::WindowFlags { Qt::WindowFlags { } },
    api::pro::WindowIcon { QIcon { } },
    api::pro::WindowIconText { QString { } },
    api::pro::WindowOpacity { 1.0 },
    api::pro::WindowRole { QString { } },
};

/// api::scope::shape 中带通用 setter 的属性
inline const auto kShapeProps = std::tuple {
    api::pro::Background { Qt::red },
    api::pro::BorderColor { Qt::black },
    api::pro::BorderWidth { 1.0 },
    api::pro::Radius { 4.0 },
};

/// api::scope::layout 的属性
inline const auto kLayoutProps = std::tuple {
    api::pro::Alignment { Qt::AlignCenter },
    api::pro::ContentsMargin { 0, 0, 0, 0 },
    api::pro::Margin { 0 },
    api::pro::Spacing { 0 },
};

/// 主题管理器：裸 manager 即 BindTheme
inline ThemeManager kThemeManager { kBlueMikuThemePack };

}
