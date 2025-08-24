/// @note
/// 本示例所使用的 ICON 为 Google 提供的 Material Icons Round
/// 字体，为正常显示，需要先下载字体并安装：
/// https://github.com/material-icons/material-icons-font/tree/master/font
/// 如果使用 Arch Linux，则可以通过 AUR 安装：ttf-material-icons-git，
/// 使用其他 Nerd Font 也是可以的

#include "creeper-qt/creeper-qt.hh"
#include <QtWidgets>

namespace short_namespace {

using namespace creeper;

namespace wipro = widget::pro;
namespace thpro = theme::pro;
namespace grpro = group::pro;
namespace lnpro = linear::pro;
namespace impro = image::pro;
namespace mwpro = main_window::pro;
namespace icpro = icon_button::pro;
namespace capro = card::pro;
namespace tbpro = text_button::pro;
}

auto main(int argc, char** argv) -> int {
    using namespace short_namespace;

    app::init {
        app::pro::Attribute { Qt::AA_EnableHighDpiScaling },
        app::pro::Attribute { Qt::AA_UseHighDpiPixmaps },
        app::pro::Complete { argc, argv },
    };

    constexpr auto avatar_url = "http://i0.hdslb.com/bfs/article/"
                                "e4e412299e6c038035241b1dc625cb62c8b5513a.jpg";

    auto avatar_image = (Image*) { nullptr };

    auto manager = ThemeManager { kBlueMikuThemePack };

    auto prop_manager = thpro::ThemeManager { manager };
    auto prop_font    = wipro::Font { "WenQuanYi Micro Hei", 12 };

    const auto NavigationSpace = [&]() noexcept {
        const auto navigation_icons_config = std::tuple {
            prop_manager,
            icpro::color::STANDARD,
            icpro::shape::DEFAULT_ROUND,
            icpro::types::TOGGLE_UNSELECTED,
            icpro::width::DEFAULT,

            // Font: "Material Icons Round"
            icpro::Font { material::round::font_1 },

            icpro::FixedSize { IconButton::kSmallContainerSize },
        };

        return lnpro::Item<FilledCard> {
            prop_manager,
            capro::Radius { 0 },

            capro::Layout<Col> {
                lnpro::SetSpacing { 10 },
                lnpro::Margin { 15 },

                lnpro::Item<Image> {
                    { 0, Qt::AlignHCenter },
                    impro::Apply { [](auto& self) {} },
                    impro::Bind { avatar_image },
                    impro::FixedSize { 60, 60 },
                    impro::Radius { -1 },
                    impro::ContentScale { ContentScale::CROP },
                    impro::BorderWidth { 3 },
                    impro::PainterResource {
                        avatar_url,
                        [] { qDebug() << "[main] Image loading completed"; },
                    },
                },
                lnpro::Spacing { 20 },
                lnpro::Item<Group<Col, IconButton>> {
                    { 0, Qt::AlignHCenter },
                    lnpro::Margin { 0 },
                    lnpro::Spacing { 10 },
                    grpro::Compose {
                        std::array {
                            std::tuple { "home", material::icon::kHome },
                            std::tuple { "settings", material::icon::kSettings },
                            std::tuple { "search", material::icon::kSearch },
                            std::tuple { "favorite", material::icon::kFavorite },
                        },
                        [&](auto name, auto icon) {
                            return new IconButton {
                                navigation_icons_config,
                                icpro::color::STANDARD,
                                icpro::FontIcon { icon },
                                icpro::Clickable { [] { } },
                            };
                        },
                        Qt::AlignHCenter,
                    },
                },
                lnpro::Spacing { 40 },
                lnpro::Stretch { 255 },
                lnpro::Item<IconButton> {
                    { 0, Qt::AlignHCenter },
                    navigation_icons_config,
                    icpro::types::DEFAULT,
                    icpro::FontIcon { material::icon::kLogout },
                    icpro::Clickable {
                        [] { QApplication::quit(); },
                    },
                },
                lnpro::Item<IconButton> {
                    { 0, Qt::AlignHCenter },
                    navigation_icons_config,
                    icpro::color::DEFAULT_FILLED,
                    icpro::FontIcon { material::icon::kDarkMode },
                    icpro::Clickable { [&] {
                        manager.toggle_color_mode();
                        manager.apply_theme();
                    } },
                },
            },
        };
    };

    const auto Workspace = [&] noexcept {
        return lnpro::Item<FilledCard> {
            { 255, Qt::AlignCenter },
            capro::Layout<Group<Col, TextButton>> {
                lnpro::Margin { 20 },
                lnpro::SetSpacing { 10 },
                lnpro::Alignment { Qt::AlignCenter },
                grpro::Compose {
                    std::array {
                        std::tuple(1, "更衣人偶"),
                        std::tuple(2, "琉璃的宝石"),
                        std::tuple(3, "彻夜之歌"),
                    },
                    [&](auto index, auto text) {
                        return new TextButton {
                            prop_manager,
                            prop_font,
                            tbpro::FixedSize { 200, 50 },
                            tbpro::Radius { -1 },
                            tbpro::Text {
                                std::format("{} {}", index, text),
                            },
                            tbpro::Clickable {
                                [](auto& self) { qDebug() << "[main] Click:" << self.text(); },
                            },
                        };
                    },
                },
                grpro::Foreach { [](TextButton& button) {
                    // 类型确定在 Group 声明时传入的模板参数
                    qDebug() << "[main]  -" << button.text();
                } },
            },
        };
    };

    /// @note 有时候 Windows 总是给我来点惊喜，
    ///       ShowWindow 这么常见命名的函数都放在全局作用域
    creeper::ShowWindow {
        [&](MainWindow& window) {
            // Q 键退出
            auto shortcut = new QShortcut { Qt::Key_Q, &window };
            shortcut->connect(shortcut, &QShortcut::activated, &app::quit);

            // 全局主题回调，受到影响的组件生命周期应与 qApp 一致
            manager.append_handler(qApp, [&](const ThemeManager& manager) {
                const auto colorscheme = manager.color_scheme();
                const auto colorborder = colorscheme.secondary_container;
                avatar_image->set_border_color(colorborder);
            });

            manager.apply_theme();
        },
        mwpro::WindowFlag { Qt::Tool },
        mwpro::Central<FilledCard> {
            prop_manager,
            capro::MinimumSize { 960, 640 },
            capro::Radius { 0 },
            capro::Level { CardLevel::HIGHEST },

            capro::Layout<Row> {
                lnpro::Margin { 0 },
                lnpro::SetSpacing { 5 },

                NavigationSpace(),
                Workspace(),
            },
        },
    };

    return app::exec();
}
