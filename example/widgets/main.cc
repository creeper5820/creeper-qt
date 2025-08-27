/// @note
/// 本示例所使用的 ICON 为 Google 提供的 Material Icons Round
/// 字体，为正常显示，需要先下载字体并安装：
/// https://github.com/material-icons/material-icons-font/tree/master/font
/// 如果使用 Arch Linux，则可以通过 AUR 安装：ttf-material-icons-git，
/// 使用其他 Nerd Font 也是可以的

#include "creeper-qt/creeper-qt.hh"
#include <QtWidgets>
#include <ranges>

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
namespace tfpro = text_field::pro;
namespace fcpro = filled_card::pro;
namespace fbpro = filled_button::pro;
}

template <typename T>
using raw_pointer = T*;

auto main(int argc, char** argv) -> int {
    using namespace short_namespace;

    app::init {
        app::pro::Attribute { Qt::AA_EnableHighDpiScaling },
        app::pro::Attribute { Qt::AA_UseHighDpiPixmaps },
        app::pro::Complete { argc, argv },
    };

    auto avatar_image = new Image {
        impro::FixedSize { 60, 60 },
        impro::Radius { -1 },
        impro::ContentScale { ContentScale::CROP },
        impro::BorderWidth { 3 },
        impro::PainterResource {
            "http://i0.hdslb.com/bfs/article/e4e412299e6c038035241b1dc625cb62c8b5513a.jpg",
            [] { qDebug() << "[main] Image loading completed"; },
        },
    };
    auto material_theme_image = raw_pointer<Image> {};

    auto manager = ThemeManager { kBlueMikuThemePack };

    auto prop_manager = thpro::ThemeManager { manager };
    auto prop_font    = wipro::Font { "WenQuanYi Micro Hei", 12 };

    auto mutable_theme_button_status = Mutable { icpro::types::TOGGLE_UNSELECTED };

    const auto NavSpace = [&]() noexcept {
        const auto navigation_icons_config = std::tuple {
            prop_manager,
            icpro::color::STANDARD,
            icpro::shape::DEFAULT_ROUND,
            icpro::types::TOGGLE_UNSELECTED,
            icpro::width::DEFAULT,

            // Font Name: "Material Icons Round"
            icpro::Font { material::round::font_1 },

            icpro::FixedSize { IconButton::kSmallContainerSize },
        };

        return new FilledCard {
            prop_manager,
            capro::Radius { 0 },

            capro::Layout<Col> {
                lnpro::SetSpacing { 10 },
                lnpro::Margin { 15 },

                lnpro::Item { { 0, Qt::AlignHCenter }, avatar_image },
                lnpro::Spacing { 20 },
                lnpro::Item<Group<Col, IconButton>> {
                    { 0, Qt::AlignHCenter },
                    lnpro::Margin { 0 },
                    lnpro::Spacing { 10 },
                    grpro::Compose {
                        std::array {
                            std::tuple { "home", material::icon::kHome },
                            std::tuple { "extension", material::icon::kExtension },
                            std::tuple { "search", material::icon::kSearch },
                            std::tuple { "settings", material::icon::kSettings },
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
                    mutable_theme_button_status,
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

    const auto MainSpace = [&] noexcept {
        return new FilledCard {
            prop_manager,
            capro::Layout<Col> {
                lnpro::Alignment { Qt::AlignTop },
                lnpro::Margin { 10 },
                lnpro::SetSpacing { 10 },

                lnpro::Item<Row> {
                    { 255 },
                    lnpro::Margin { 20 },

                    lnpro::Item<Image> {
                        impro::Bind { material_theme_image },
                        impro::FixedHeight { 500 },
                        impro::ContentScale { ContentScale::CROP },
                        impro::BorderWidth { 5 },
                        impro::PainterResource {
                            "https://lh3.googleusercontent.com/"
                            "1U71piLcGDg-O2CUVDHaShSo-igfXvZzsdu4e_u_Tt8VuAAPPAJFzcMEGE8bTfu20-"
                            "BVvVS9v2kDSZ9BcDc672z8IVmjsCApRA6gT1e8dpMoYx5BFQ=w1800-rj",
                        },
                    },
                },
            },
        };
    };

    const auto ContentSpace = [&] noexcept {
        const auto ButtonGroup = new Group<Col, TextButton> {
            lnpro::Alignment { Qt::AlignTop | Qt::AlignHCenter },
            grpro::Compose {
                std::views::iota('A', 'Z' + 1),
                [&](char c) {
                    return new TextButton {
                        prop_manager,
                        tbpro::FixedWidth { 200 },
                        tbpro::FixedHeight { 50 },
                        tbpro::Radius { -1 },
                        tbpro::Text {
                            std::format("{}.{}{}{}", int { c - 'A' }, c, c, c),
                        },
                        tbpro::Font { "JetBrains Mono" },
                        tbpro::Clickable { [](TextButton& button) {
                            qDebug() << "[main] Clicked" << button.text();
                        } },
                    };
                },
                Qt::AlignTop | Qt::AlignHCenter,
            },
        };
        return new FilledCard {
            prop_manager,
            fcpro::Radius { 10 },
            fcpro::Layout<Col> {
                lnpro::Item<ScrollArea> {
                    prop_manager,
                    scroll::pro::ScrollBarPolicy {
                        Qt::ScrollBarAlwaysOff,
                        Qt::ScrollBarAlwaysOff,
                    },
                    scroll::pro::Item<Widget> {
                        wipro::Layout { ButtonGroup },
                    },
                },
            },
        };
    };

    /// @note 有时候 Windows 总是给我来点惊喜，
    ///       ShowWindow 这么常见命名的函数都放在全局作用域
    creeper::ShowWindow {
        [&](MainWindow& window) {
            // Q 键退出
            QObject::connect(
                new QShortcut { Qt::Key_Q, &window }, &QShortcut::activated, &app::quit);

            // E 键换主题
            QObject::connect(new QShortcut { Qt::Key_E, &window }, &QShortcut::activated, [&] {
                manager.toggle_color_mode();
                manager.apply_theme();

                mutable_theme_button_status = (manager.color_mode() == ColorMode::LIGHT)
                    ? icpro::types::TOGGLE_UNSELECTED
                    : icpro::types::TOGGLE_SELECTED;
            });

            // 全局主题回调，受到影响的组件生命周期应与 qApp 一致
            manager.append_handler(qApp, [&](const ThemeManager& manager) {
                const auto colorscheme = manager.color_scheme();
                const auto colorborder = colorscheme.secondary_container;
                avatar_image->set_border_color(colorborder);
                material_theme_image->set_border_color(colorborder);
            });

            manager.apply_theme();
        },
        mwpro::MinimumSize { 800, 600 },
        mwpro::Central<FilledCard> {
            prop_manager,
            capro::Radius { 0 },
            capro::Level { CardLevel::HIGHEST },

            capro::Layout<Row> {
                lnpro::Margin { 0 },
                lnpro::SetSpacing { 0 },

                lnpro::Item { NavSpace() },
                lnpro::Item<Col> {
                    lnpro::ContentsMargin { 15, 15, 5, 15 },
                    lnpro::Item { ContentSpace() },
                },
                lnpro::Item<Col> {
                    { 255 },
                    lnpro::ContentsMargin { 5, 15, 15, 15 },
                    lnpro::Item { MainSpace() },
                },
            },
        },
    };

    return app::exec();
}
