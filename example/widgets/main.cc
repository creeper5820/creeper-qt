#include "creeper-qt/creeper-qt.hh"
#include <QtWidgets>

auto main(int argc, char** argv) -> int {
    using namespace creeper;

    namespace wipro = widget::pro;
    namespace thpro = theme::pro;
    namespace lnpro = linear::pro;
    namespace impro = image::pro;
    namespace mwpro = main_window::pro;
    namespace icpro = icon_button::pro;
    namespace capro = card::pro;

    app::init {
        app::pro::Attribute { Qt::AA_EnableHighDpiScaling },
        app::pro::Attribute { Qt::AA_UseHighDpiPixmaps },
        app::pro::Complete { argc, argv },
    };

    auto avatar_image = (Image*) {};

    constexpr auto avatar_url = "http://i0.hdslb.com/bfs/article/"
                                "e4e412299e6c038035241b1dc625cb62c8b5513a.jpg";

    auto manager = ThemeManager { kBlueMikuThemePack };

    auto manager_config = thpro::ThemeManager { manager };
    auto font_config    = wipro::Font { "WenQuanYi Micro Hei", 12 };

    const auto NavigationSpace = [&]() noexcept {
        const auto navigation_icons_config = std::tuple {
            manager_config,
            icpro::color::STANDARD,
            icpro::shape::DEFAULT_ROUND,
            icpro::types::TOGGLE_UNSELECTED,
            icpro::width::DEFAULT,
            icpro::FontIcon { material::icon::kCircle },
            icpro::Font { material::round::font_1 },
            icpro::FixedSize { IconButton::kSmallContainerSize },
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
        return lnpro::Item<FilledCard> {
            manager_config,
            capro::Radius { 0 },

            capro::Layout<Col> {
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
        };
    };

    const auto Workspace = [&] noexcept {
        return lnpro::Item<FilledCard> {
            { 255 },
            capro::Layout<Group<Col, TextButton>> {
                col::pro::Margin { 20 },
                col::pro::SetSpacing { 10 },
                col::pro::Alignment { Qt::AlignTop },
                group::pro::Compose {
                    std::array {
                        std::tuple(1, "更衣人偶"),
                        std::tuple(2, "琉璃的宝石"),
                        std::tuple(3, "彻夜之歌"),
                    },
                    [&](auto index, auto text) {
                        return new TextButton {
                            manager_config,
                            font_config,
                            text_button::pro::FixedSize { 200, 50 },
                            text_button::pro::Radius { -1 },
                            text_button::pro::Text {
                                std::format("{} {}", index, text),
                            },
                            text_button::pro::Clickable {
                                [](auto& self) { qDebug() << "Click:" << self.text(); },
                            },
                        };
                    },
                },
                group::pro::Foreach { [](TextButton& button) {
                    // 类型确定在 Group 声明时传入的模板参数
                    qDebug() << "  -" << button.text();
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
        mwpro::WindowFlag { Qt::Tool },
        mwpro::Central<FilledCard> {
            manager_config,
            capro::MinimumSize { 1440, 960 },
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
