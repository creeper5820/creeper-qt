#include <creeper-qt/core/application.hh>
#include <creeper-qt/layout/group.hh>
#include <creeper-qt/layout/linear.hh>
#include <creeper-qt/layout/mutual-exclusion-group.hh>
#include <creeper-qt/utility/material-icon.hh>
#include <creeper-qt/utility/theme/theme.hh>
#include <creeper-qt/widget/buttons/icon-button.hh>
#include <creeper-qt/widget/cards/filled-card.hh>
#include <creeper-qt/widget/image.hh>

using namespace creeper;
namespace fc = filled_card::pro;
namespace sg = select_group::pro;
namespace ln = linear::pro;
namespace im = image::pro;
namespace ic = icon_button::pro;

auto NavComponent(ThemeManager& manager,
    std::function<void(const std::string_view&)> switch_view_callback) noexcept {

    auto avatar_image = new Image {
        im::FixedSize { 60, 60 },
        im::Radius { -1 },
        im::ContentScale { ContentScale::CROP },
        im::BorderWidth { 3 },
        im::PainterResource {
            "http://i0.hdslb.com/bfs/article/e4e412299e6c038035241b1dc625cb62c8b5513a.jpg",
            [] { qDebug() << "[main] Image loading completed"; },
        },
    };
    manager.append_handler(avatar_image, [avatar_image](const ThemeManager& manager) {
        const auto colorscheme = manager.color_scheme();
        const auto colorborder = colorscheme.secondary_container;
        avatar_image->set_border_color(colorborder);
    });

    const auto navigation_icons_config = std::tuple {
        ic::ThemeManager { manager },
        ic::ColorStandard,
        ic::ShapeRound,
        ic::TypesToggleUnselected,
        ic::WidthDefault,
        ic::Font { material::round::font_1 },
        ic::FixedSize { IconButton::kSmallContainerSize },
    };

    return new FilledCard {
        fc::ThemeManager { manager },
        fc::Radius { 0 },
        fc::Level { CardLevel::HIGHEST },

        fc::Layout<Col> {
            ln::Spacing { 10 },
            ln::Margin { 15 },

            ln::Item { { 0, Qt::AlignHCenter }, avatar_image },
            ln::SpacingItem { 20 },
            ln::Item<SelectGroup<Col, IconButton>> {
                { 0, Qt::AlignHCenter },
                ln::Margin { 0 },
                ln::SpacingItem { 10 },
                sg::Compose {
                    std::array {
                        std::tuple { "home", material::icon::kHome },
                        std::tuple { "extension", material::icon::kExtension },
                        std::tuple { "search", material::icon::kSearch },
                        std::tuple { "settings", material::icon::kSettings },
                    },
                    [&](std::string_view name, auto icon) {
                        const auto status = (name == "home") //
                            ? ic::TypesToggleSelected
                            : ic::TypesToggleUnselected;
                        return new IconButton {
                            navigation_icons_config,
                            status,
                            ic::ColorStandard,
                            ic::FontIcon { icon },
                            ic::Clickable { [=] { switch_view_callback(name); } },
                        };
                    },
                    Qt::AlignHCenter,
                },
                sg::SignalInjection { &IconButton::clicked },
            },
            ln::SpacingItem { 40 },
            ln::Stretch { 255 },
            ln::Item<IconButton> {
                { 0, Qt::AlignHCenter },
                navigation_icons_config,
                ic::TypesDefault,
                ic::FontIcon { material::icon::kLogout },
                ic::Clickable { &app::quit },
            },
            ln::Item<IconButton> {
                { 0, Qt::AlignHCenter },
                navigation_icons_config,
                ic::ColorFilled,
                ic::FontIcon { material::icon::kDarkMode },
                ic::Clickable { [&] {
                    manager.toggle_color_mode();
                    manager.apply_theme();
                } },
            },
        },
    };
}
