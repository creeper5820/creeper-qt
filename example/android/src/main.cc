/// creeper-qt Android 示例：Material 3 手机布局。
///
/// 页面自上而下为顶栏、内容区（可滚动卡片墙 + 悬浮工具条）与底部导航。
/// 导航选中态由 MutableValue 驱动，悬浮工具条最右侧按钮轮切主题。

#include "theme.hh"

#include <QFontDatabase>

#include <creeper-qt/core/application.hh>
#include <creeper-qt/layout/grid.hh>
#include <creeper-qt/layout/linear.hh>
#include <creeper-qt/layout/scroll.hh>
#include <creeper-qt/utility/material-icon.hh>
#include <creeper-qt/utility/theme/preset/blue-miku.hh>
#include <creeper-qt/utility/theme/preset/gloden-harvest.hh>
#include <creeper-qt/utility/theme/preset/green.hh>
#include <creeper-qt/utility/theme/theme.hh>
#include <creeper-qt/utility/wrapper/mutable-value.hh>
#include <creeper-qt/widget/buttons/icon-button.hh>
#include <creeper-qt/widget/buttons/outlined-button.hh>
#include <creeper-qt/widget/cards/elevated-card.hh>
#include <creeper-qt/widget/cards/filled-card.hh>
#include <creeper-qt/widget/cards/outlined-card.hh>
#include <creeper-qt/widget/main-window.hh>
#include <creeper-qt/widget/shape/rounded-rect.hh>
#include <creeper-qt/widget/text.hh>
#include <creeper-qt/widget/widget.hh>

#include <array>
#include <ranges>

using namespace creeper;

namespace ecp = elevated_card::pro;
namespace fcp = filled_card::pro;
namespace gp  = grid::pro;
namespace ibp = icon_button::pro;
namespace ln  = linear::pro;
namespace mwp = main_window::pro;
namespace obp = outlined_button::pro;
namespace ocp = outlined_card::pro;
namespace rrp = rounded_rect::pro;
namespace scp = scroll::pro;
namespace txp = text::pro;
namespace wgp = widget::pro;

namespace {

/// Grid 尚未提供 Apply，补一个与 widget::pro::Apply 同义的 prop
template <typename F>
struct GridApply : gp::Token {
    F function;
    explicit GridApply(F function) noexcept
        : function { std::move(function) } { }
    auto apply(QGridLayout& self) const noexcept -> void { function(self); }
};

/// props 列表直接接受 range：按数据逐项生成 prop
/// （Util::ForEach 的带下标重载依赖 std::views::enumerate，NDK 的 libc++ 未提供）
template <std::ranges::range R, typename Generator>
auto each(const R& range, Generator generator) noexcept {
    return range | std::views::transform(std::move(generator));
}

template <class T>
using GridCell = typename gp::Item<T>::LayoutMethod;

auto icon_font(int px) noexcept -> QFont {
    auto font = QFont { material::outlined::font };
    font.setPixelSize(px);
    return font;
}

auto text_font(int px, QFont::Weight weight = QFont::Normal) noexcept -> QFont {
    auto font = QFont {};
    font.setPixelSize(px);
    font.setWeight(weight);
    return font;
}

/// 主题轮切：基线紫 → 蓝色初音 → 丰收金 → 橄榄绿
class ThemeCycle {
public:
    explicit ThemeCycle(ThemeManager& manager) noexcept
        : manager { manager } { }

    auto next() noexcept -> void {
        index = (index + 1) % kPacks.size();
        manager.set_theme_pack(kPacks[index]);
        manager.apply_theme();
    }

private:
    static constexpr auto kPacks = std::array {
        kMaterialBaselineThemePack,
        kBlueMikuThemePack,
        kGoldenHarvestThemePack,
        kGreenThemePack,
    };
    ThemeManager& manager;
    std::size_t index = 0;
};

// ---- 卡片墙：{row, column, row_span, column_span} ----
struct Tile {
    int row, column, row_span, column_span;
};
constexpr auto kTiles = std::array {
    Tile { 0, 0, 1, 2 },
    Tile { 0, 2, 2, 1 },
    Tile { 1, 0, 1, 1 },
    Tile { 1, 1, 1, 1 },
    Tile { 2, 0, 1, 1 },
    Tile { 2, 1, 1, 1 },
    Tile { 2, 2, 1, 1 },
    Tile { 3, 0, 1, 1 },
    Tile { 3, 1, 1, 2 },
    Tile { 4, 0, 1, 2 },
    Tile { 4, 2, 1, 1 },
};
constexpr auto kTileRows    = 5;
constexpr auto kTileColumns = 3;

// ---- 底部导航 ----
struct Destination {
    int index;
    const char* icon;
    const char* label;
};
constexpr auto kDestinations = std::array {
    Destination { 0, "stars", "Label" },
    Destination { 1, "stars", "Label" },
    Destination { 2, "stars", "Label" },
};

/// 顶栏：菜单 + 胶囊按钮 + 头像
auto TopBar(ThemeManager& manager) noexcept {
    return ln::Item<Row> {
        { 0, Qt::Alignment {} },
        ln::ContentsMargin { { 4, 8, 12, 8 } },
        ln::Spacing { 4 },

        ln::Item<IconButton> {
            { 0, Qt::AlignVCenter },
            // IconButton 按当前尺寸计算圆角，FixedSize 须先于形状类 prop
            ibp::FixedSize { 48, 48 },
            ibp::ThemeManager { manager },
            ibp::ColorStandard,
            ibp::ShapeRound,
            ibp::Font { icon_font(20) },
            ibp::FontIcon { material::icon::kMenu },
        },
        ln::Item<OutlinedButton> {
            { 1, Qt::AlignVCenter },
            obp::FixedHeight { 48 },
            obp::Radius { 24 },
            obp::Font { text_font(16) },
            obp::Text { "Label" },
            obp::Apply { [&manager](OutlinedButton& self) {
                manager.append_handler(&self, [&self](const ThemeManager& manager) {
                    const auto& scheme = manager.color_scheme();
                    self.set_color_scheme(scheme);
                    self.set_border_width(1);
                    self.set_border_color(scheme.outline);
                    self.set_text_color(scheme.on_surface_variant);
                });
            } },
        },
        ln::Item<RoundedRect> {
            { 0, Qt::AlignVCenter },
            rrp::FixedSize { 32, 32 },
            rrp::Radius { 16 },
            rrp::Background { Qt::transparent },
            rrp::BorderWidth { 1 },
            rrp::Apply { [&manager](RoundedRect& self) {
                manager.append_handler(&self, [&self](const ThemeManager& manager) {
                    const auto& scheme = manager.color_scheme();
                    // RoundedRect 继承自 Shape，setter 不触发重绘，须显式 update
                    self.set_background(Qt::transparent);
                    self.set_border_color(scheme.outline);
                    self.update();
                });
            } },
        },
    };
}

/// 卡片墙：5 行 3 列等宽，作为 ScrollArea 的内容
auto CardWall(ThemeManager& manager) noexcept {
    const auto grid_metrics = [](QGridLayout& layout) {
        layout.setSpacing(6);
        // 底部留白让悬浮工具条越过最后一行卡片
        layout.setContentsMargins(8, 0, 8, 16);
        for (auto column = 0; column < kTileColumns; ++column)
            layout.setColumnStretch(column, 1);
        for (auto row = 0; row < kTileRows; ++row)
            layout.setRowStretch(row, 1);
    };

    return scp::Item<Widget> {
        // QScrollArea 在 setWidgetResizable(true) 下按内容控件的 size hint 拉伸，
        // 必须显式 Expanding 才能铺满视口，并让内层 Grid 拿到完整高度
        wgp::SizePolicy { QSizePolicy::Expanding, QSizePolicy::Expanding },
        wgp::Layout<Grid> {
            GridApply { grid_metrics },
            each(kTiles,
                [&](const Tile& tile) {
                    return gp::Item<OutlinedCard> {
                        GridCell<OutlinedCard> {
                            tile.row, tile.column, tile.row_span, tile.column_span },
                        // 卡片自身也要 Expanding，行/列 stretch 才有可分配的余量
                        ocp::SizePolicy { QSizePolicy::Expanding, QSizePolicy::Expanding },
                        // 最小尺寸兜底：空卡片 size hint 接近 0，防止行被压扁成横条
                        ocp::MinimumSize { 64, 64 },
                        ocp::Level { CardLevel::LOWEST },
                        ocp::ThemeManager { manager },
                        ocp::Radius { 4 },
                        ocp::BorderWidth { 1 },
                    };
                }),
        },
    };
}

/// 悬浮工具条：三个动作按钮的胶囊 + 主题轮切按钮
auto FloatingToolbar(ThemeManager& manager, ThemeCycle& cycle) noexcept {
    /// ElevatedCard 继承自 Shape，set_background/set_border_color 不触发重绘，
    /// surface_container_low 也不在 Card::set_color_scheme 的 level 映射里，
    /// 因此主题变化时需要自己 setter 并 update。
    const auto tint = [&manager](ElevatedCard& self) {
        manager.append_handler(&self, [&self](const ThemeManager& manager) {
            const auto& scheme = manager.color_scheme();
            self.set_color_scheme(scheme);
            self.set_background(scheme.surface_container_low);
            self.set_border_color(scheme.outline);
            self.update();
        });
    };
    const auto button = std::tuple {
        ibp::FixedSize { 40, 40 },
        ibp::ThemeManager { manager },
        ibp::ColorStandard,
        ibp::ShapeRound,
        ibp::Font { icon_font(20) },
    };

    return gp::Item<Row> {
        GridCell<Row> { 0, 0, Qt::AlignBottom | Qt::AlignHCenter },
        ln::Spacing { 14 },

        ln::Item<ElevatedCard> {
            { 0, Qt::AlignVCenter },
            ecp::FixedHeight { 52 },
            ecp::Radius { 26 },
            ecp::BorderWidth { 1.5 },
            ecp::Apply { tint },
            ecp::Layout<Row> {
                ln::ContentsMargin { { 8, 6, 8, 6 } },
                ln::Spacing { 4 },
                each(std::array { "stars", "stars", "stars" },
                    [&](const char* icon) {
                        return ln::Item<IconButton> { button, ibp::FontIcon { icon } };
                    }),
            },
        },
        ln::Item<ElevatedCard> {
            { 0, Qt::AlignVCenter },
            ecp::FixedSize { 46, 46 },
            ecp::Radius { 12 },
            ecp::BorderWidth { 1.5 },
            ecp::Apply { tint },
            ecp::Layout<Row> {
                ln::Margin { 0 },
                ln::Alignment { Qt::AlignCenter },
                ln::Item<IconButton> {
                    button,
                    ibp::FontIcon { "palette" },
                    ibp::Clickable { [&cycle] { cycle.next(); } },
                },
            },
        },
    };
}

/// 底部导航：选中项由 selected 驱动，按钮只负责写入
auto NavigationBar(ThemeManager& manager, MutableValue<int>& selected) noexcept {
    return ln::Item<Row> {
        { 0, Qt::Alignment {} },
        ln::ContentsMargin { { 0, 12, 0, 16 } },

        each(kDestinations,
            [&](const Destination& destination) {
                const auto index = destination.index;

                const auto indicate = [&manager, index](IconButton& self, int current) {
                    self.set_color(
                        index == current ? IconButton::Color::TONAL : IconButton::Color::STANDARD);
                    self.set_color_scheme(manager.color_scheme());
                };
                const auto emphasize = [index](Text& self, int current) {
                    self.setFont(text_font(12, index == current ? QFont::Bold : QFont::Normal));
                };

                return ln::Item<Col> {
                    { 1, Qt::AlignVCenter },
                    ln::Alignment { Qt::AlignCenter },
                    ln::Spacing { 4 },

                    ln::Item<IconButton> {
                        { 0, Qt::AlignHCenter },
                        ibp::FixedSize { 40, 32 },
                        ibp::ThemeManager { manager },
                        ibp::TypesDefault,
                        ibp::ShapeRound,
                        ibp::WidthWide,
                        MutableTransform { indicate, selected },
                        ibp::Font { icon_font(22) },
                        ibp::FontIcon { destination.icon },
                        ibp::Clickable { [&selected, index] { selected = index; } },
                    },
                    ln::Item<Text> {
                        { 0, Qt::AlignHCenter },
                        MutableTransform { emphasize, selected },
                        txp::Alignment { Qt::AlignCenter },
                        txp::Text { destination.label },
                        txp::Apply { [&manager](Text& self) {
                            manager.append_handler(&self, [&self](const ThemeManager& manager) {
                                self.set_color(manager.color_scheme().on_surface_variant);
                            });
                        } },
                    },
                };
            }),
    };
}

}

auto main(int argc, char** argv) -> int {
    app::init {
        app::pro::Complete { argc, argv },
    };

    for (const auto* font :
        { ":/fonts/MaterialIconsRound-Regular.otf", ":/fonts/MaterialIconsOutlined-Regular.otf" })
        if (QFontDatabase::addApplicationFont(font) == -1)
            qWarning() << "无法加载图标字体：" << font;

    auto manager  = ThemeManager { kMaterialBaselineThemePack };
    auto cycle    = ThemeCycle { manager };
    auto selected = MutableValue<int> { 0 };

    ShowWindow<MainWindow> {
        [](MainWindow& window) { window.setWindowTitle("creeper-qt | Material 3"); },
        mwp::Central<FilledCard> {
            fcp::Radius { 0 },
            fcp::Apply { [&manager](FilledCard& self) {
                manager.append_handler(&self, [&self](const ThemeManager& manager) {
                    self.set_background(manager.color_scheme().surface);
                    self.update();
                });
            } },
            fcp::Layout<Col> {
                ln::Margin { 0 },
                ln::Spacing { 0 },

                TopBar(manager),
                ln::Item<Grid> {
                    { 1, Qt::Alignment {} },
                    gp::Item<ScrollArea> {
                        GridCell<ScrollArea> { 0, 0 },
                        scp::ThemeManager { manager },
                        // ScrollArea 在 Grid 中纵向撑满，内容区才能拿到完整高度
                        scp::SizePolicy { QSizePolicy::Expanding, QSizePolicy::Expanding },
                        scp::VerticalScrollBarPolicy { Qt::ScrollBarAlwaysOff },
                        scp::HorizontalScrollBarPolicy { Qt::ScrollBarAlwaysOff },
                        CardWall(manager),
                    },
                    FloatingToolbar(manager, cycle),
                },
                NavigationBar(manager, selected),
            },
        },
    };

    manager.apply_theme();
    return app::exec();
}
