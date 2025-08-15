#include "creeper-qt/creeper-qt.hh"
#include <QtWidgets>

auto main(int argc, char** argv) -> int {
    using namespace creeper;

    namespace lnpro = linear::pro;
    namespace impro = image::pro;
    namespace wipro = main_window::pro;

    auto avatar_image = (Image*) {};

    constexpr auto avatar_url //
        = "http://i0.hdslb.com/bfs/article/e4e412299e6c038035241b1dc625cb62c8b5513a.jpg";

    auto manager = ThemeManager { kBlueMikuThemePack };

    auto manager_config = theme::pro::ThemeManager { manager };
    auto font_config    = widget::pro::Font { "WenQuanYi Micro Hei", 12 };

    const auto outlined_button_config = std::tuple {
        manager_config,
        font_config,
        button::pro::FixedSize { 100, 50 },
        button::pro::Radius { -1 },
        button::pro::BorderWidth { 0.5 },
    };
    const auto navigation_icons_config = std::tuple {
        manager_config,
        icon_button::pro::color::STANDARD,
        icon_button::pro::shape::DEFAULT_ROUND,
        icon_button::pro::types::TOGGLE_UNSELECTED,
        icon_button::pro::width::DEFAULT,
        icon_button::pro::FontIcon { material::icon::kCircle },
        icon_button::pro::Font { material::round::font_1 },
        icon_button::pro::FixedSize { IconButton::kSmallContainerSize },
    };

    const auto NavigationButton = [&](auto icon, auto f) {
        return lnpro::Item<IconButton> {
            { 0, Qt::AlignHCenter },
            navigation_icons_config,
            icon_button::pro::color::STANDARD,
            icon_button::pro::FontIcon { icon },
            icon_button::pro::Clickable { f },
        };
    };

    app::init {
        app::pro::Attribute { Qt::AA_EnableHighDpiScaling },
        app::pro::Attribute { Qt::AA_UseHighDpiPixmaps },
        app::pro::Complete { argc, argv },
    };
    ShowWindow {
        [&](MainWindow& window) {
            // Q 键退出
            auto shortcut = new QShortcut { Qt::Key_Q, &window };
            shortcut->connect(shortcut, &QShortcut::activated, [&]() {
                auto focused = QApplication::focusWidget();
                QApplication::quit();
            });
            // 全局主题回调，收到影响的组件生命周期应与 qApp 一致
            manager.append_handler(qApp, [&](const ThemeManager& manager) {
                const auto colorscheme = manager.color_scheme();
                const auto colorborder = colorscheme.secondary_container;
                avatar_image->set_border_color(colorborder);
            });

            manager.apply_theme();
        },
        wipro::WindowFlag { Qt::Tool },
        wipro::Central<FilledCard> {
            manager_config,
            card::pro::MinimumSize { 1440, 960 },
            card::pro::Radius { 0 },
            card::pro::Level { CardLevel::HIGHEST },

            card::pro::Layout<Row> {
                lnpro::Margin { 0 },
                lnpro::SetSpacing { 5 },

                lnpro::Item<FilledCard> {
                    manager_config,
                    card::pro::Radius { 0 },

                    card::pro::Layout<Col> {
                        lnpro::SetSpacing { 10 },
                        lnpro::Margin { 15 },

                        lnpro::Item<Image> {
                            { 0, Qt::AlignHCenter },
                            impro::Bind { avatar_image },
                            impro::FixedSize { 60, 60 },
                            impro::Radius { -1 },
                            impro::ContentScale { ContentScale::CROP },
                            impro::BorderWidth { 3 },
                            impro::PainterResource {
                                avatar_url,
                                [&] { avatar_image->update(); },
                            },
                        },
                        lnpro::Spacing { 20 },
                        NavigationButton(material::icon::kHome, [] {}),
                        NavigationButton(material::icon::kSettings, [] {}),
                        NavigationButton(material::icon::kSearch, [] {}),
                        NavigationButton(material::icon::kFavorite, [] {}),
                        lnpro::Spacing { 40 },
                        lnpro::Stretch { 255 },
                        lnpro::Item<IconButton> {
                            { 0, Qt::AlignHCenter },
                            navigation_icons_config,
                            icon_button::pro::types::DEFAULT,
                            icon_button::pro::FontIcon { material::icon::kLogout },
                            icon_button::pro::Clickable {
                                [] { QApplication::quit(); },
                            },
                        },
                        lnpro::Item<IconButton> {
                            { 0, Qt::AlignHCenter },
                            navigation_icons_config,
                            icon_button::pro::color::DEFAULT_FILLED,
                            icon_button::pro::FontIcon { material::icon::kDarkMode },
                            icon_button::pro::Clickable { [&] {
                                manager.toggle_color_mode();
                                manager.apply_theme();
                            } },
                        },
                    },
                },
                lnpro::Item<FilledCard> {
                    { 255 },
                    card::pro::Layout<Col> {},
                },
            },
        },
    };
    return app::exec();
}
