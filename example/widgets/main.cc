#include "creeper-qt/creeper-qt.hh"
#include "creeper-qt/widget/text.hh"
#include <QtWidgets>
#include <qnamespace.h>

auto main(int argc, char** argv) -> int {
    using namespace creeper;

    namespace impro = image::pro;
    namespace lnpro = linear::pro;

    constexpr auto avatar_url //
        = "http://i0.hdslb.com/bfs/article/e4e412299e6c038035241b1dc625cb62c8b5513a.jpg";

    auto manager = ThemeManager { kBlueMikuThemePack };

    auto theme_manager = util::theme::pro::ThemeManager { manager };
    auto font          = widget::pro::Font { "WenQuanYi Micro Hei", 12 };

    const auto outlined_button = std::tuple {
        theme_manager,
        font,
        button::pro::FixedSize { 100, 50 },
        button::pro::Radius { -1 },
        button::pro::BorderWidth { 0.5 },
    };
    const auto navigation_icons = std::tuple {
        theme_manager,
        icon_button::pro::color::TONAL,
        icon_button::pro::shape::DEFAULT_ROUND,
        icon_button::pro::types::DEFAULT,
        icon_button::pro::width::WIDE,
        icon_button::pro::FontIcon { material::icon::kCircle },
        icon_button::pro::Font { material::outlined::font, 25 },
        icon_button::pro::FixedSize { 80, 60 },
    };

    app::init {
        app::pro::Attribute { Qt::AA_EnableHighDpiScaling },
        app::pro::Attribute { Qt::AA_UseHighDpiPixmaps },
        app::pro::Complete { argc, argv },
    };

    auto avatar_image = (Image*) {};

    const auto workspace_navigation = new FilledCard {
        theme_manager,
        card::pro::Radius { 0 },

        card::pro::Layout<Col> {
            lnpro::SetSpacing { 10 },

            lnpro::Spacing { 20 },
            lnpro::Item<Image> {
                { 0, Qt::AlignHCenter },
                impro::Bind { avatar_image },
                impro::FixedSize { 80, 80 },
                impro::Radius { -1 },
                impro::ContentScale { ContentScale::CROP },
                impro::BorderWidth { 3 },
                impro::PainterResource {
                    avatar_url,
                    [&] { avatar_image->update(); },
                },
            },
            lnpro::Item<Text> {
                { 0, Qt::AlignHCenter },
                theme_manager,
                font,
                text::pro::FixedWidth { 80 },
                text::pro::Text { "CREEPER" },
                text::pro::WordWrap { true },
                text::pro::Alignment { Qt::AlignCenter },
            },
            lnpro::Spacing { 40 },
            lnpro::Item<IconButton> {
                { 0, Qt::AlignHCenter },
                navigation_icons,
                icon_button::pro::FontIcon { material::icon::kHome },
            },
            lnpro::Item<IconButton> {
                { 0, Qt::AlignHCenter },
                navigation_icons,
                icon_button::pro::FontIcon { material::icon::kCancel },
            },
            lnpro::Item<IconButton> {
                { 0, Qt::AlignHCenter },
                navigation_icons,
                icon_button::pro::FontIcon { material::icon::kAttachFile },
            },
            lnpro::Item<IconButton> {
                { 0, Qt::AlignHCenter },
                navigation_icons,
                icon_button::pro::FontIcon { material::icon::kDashboard },
            },
            lnpro::Stretch {
                255,
            },
            lnpro::Item<OutlinedButton> {
                outlined_button,
                button::pro::Text { "退出程序" },
                button::pro::Clickable {
                    [] { QApplication::quit(); },
                },
            },
            lnpro::Item<OutlinedButton> {
                outlined_button,
                button::pro::Text { "颜色模式" },
                button::pro::Clickable { [&] {
                    manager.toggle_color_mode();
                    manager.apply_theme();
                } },
            },
        },
    };

    auto image = (Image*) {};

    const auto background = new FilledCard {
        theme_manager,
        card::pro::MinimumSize { 720, 480 },
        card::pro::Radius { 0 },
        card::pro::Level { CardLevel::HIGHEST },

        card::pro::Layout<Row> {
            lnpro::Margin { 0 },
            lnpro::SetSpacing { 5 },

            lnpro::Item { workspace_navigation },
            lnpro::Item<FilledCard> {
                { 255 },
                card::pro::Layout<Col> {},
            },
        },
    };

    auto window = new QMainWindow {};
    window->setCentralWidget(background);
    window->show();

    manager.append_handler(avatar_image, [&](const ThemeManager& manager) {
        const auto colorscheme = manager.color_scheme();
        const auto colorborder = colorscheme.secondary_container;
        avatar_image->set_border_color(colorborder);
    });

    manager.apply_theme();
    return app::exec();
}
