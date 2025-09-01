#include <creeper-qt/layout/linear.hh>
#include <creeper-qt/utility/material-icon.hh>
#include <creeper-qt/widget/buttons/icon-button.hh>
#include <creeper-qt/widget/cards/filled-card.hh>
#include <creeper-qt/widget/image.hh>
#include <creeper-qt/widget/shape/wave-circle.hh>
#include <creeper-qt/widget/text-fields.hh>

using namespace creeper;
namespace capro = card::pro;
namespace lnpro = linear::pro;
namespace impro = image::pro;
namespace ibpro = icon_button::pro;

static auto SearchComponent(ThemeManager& manager) noexcept {
    return new Row {
        lnpro::Item<FilledCard> {
            capro::ThemeManager { manager },
            capro::FixedHeight { 40 },
            capro::Radius { -1 },
            capro::Level { CardLevel::HIGHEST },
        },
        lnpro::Spacing { 20 },
        lnpro::Item<IconButton> {
            ibpro::ThemeManager { manager },
            ibpro::FixedSize { 40, 40 },
            ibpro::Color { IconButton::Color::TONAL },
            ibpro::Font { material::kRoundSmallFont },
            ibpro::FontIcon { material::icon::kAdd },
        },
        lnpro::Item<IconButton> {
            ibpro::ThemeManager { manager },
            ibpro::FixedSize { 40, 40 },
            ibpro::Color { IconButton::Color::TONAL },
            ibpro::Font { material::kRoundSmallFont },
            ibpro::FontIcon { material::icon::kFavorite },
        },
        lnpro::Item<IconButton> {
            ibpro::ThemeManager { manager },
            ibpro::FixedSize { 40, 40 },
            ibpro::Color { IconButton::Color::TONAL },
            ibpro::Font { material::kRoundSmallFont },
            ibpro::FontIcon { material::icon::kFolder },
        },
    };
}
static auto ItemComponent(ThemeManager& manager) noexcept {
    return new Widget {
        widget::pro::Layout<Col> {
            col::pro::Alignment { Qt::AlignTop | Qt::AlignLeft },
            col::pro::SetSpacing { 5 },
            col::pro::Margin { 10 },

            col::pro::Item<WaveCircle> {
                wave_circle::pro::FixedSize { 150, 200 },
                wave_circle::pro::FlangeNumber { 8 },
                wave_circle::pro::FlangeRadius { 20 },
                wave_circle::pro::OverallRadius { 75 },
                wave_circle::pro::ProtrudingRatio { 0.8 },
                wave_circle::pro::BorderColor { Qt::transparent },
                wave_circle::pro::Apply { [&manager](WaveCircle& self) {
                    manager.append_handler(&self, [&](const ThemeManager& manager) {
                        const auto colorscheme = manager.color_scheme();
                        const auto colorborder = colorscheme.surface_container_lowest;
                        self.set_background(colorborder);
                    });
                } },
            },
            col::pro::Item<FilledCard> {
                card::pro::ThemeManager { manager },
                card::pro::Level { CardLevel::LOW },
                card::pro::FixedSize { 150, 30 },
            },
            col::pro::Item<FilledCard> {
                card::pro::ThemeManager { manager },
                card::pro::Level { CardLevel::LOW },
                card::pro::FixedSize { 100, 30 },
            },
        },
    };
}
static auto BannerComponent(ThemeManager& manager) noexcept {
    return new Image {
        impro::ContentScale { ContentScale::CROP },
        impro::SizePolicy { QSizePolicy::Expanding },
        impro::BorderWidth { 5 },
        impro::FixedHeight { 300 },
        impro::MaximumWidth { 750 },
        impro::PainterResource {
            "https://c-ssl.duitang.com/uploads/blog/202103/16/20210316112119_181c8.jpeg",
        },
        impro::Apply { [&manager](Image& self) {
            manager.append_handler(&self, [&](const ThemeManager& manager) {
                const auto colorscheme = manager.color_scheme();
                const auto colorborder = colorscheme.secondary_container;
                self.set_border_color(colorborder);
            });
        } },
    };
}

auto ViewComponent(ThemeManager& manager) noexcept {
    return new FilledCard {
        capro::ThemeManager { manager },
        capro::SizePolicy { QSizePolicy::Expanding },
        capro::Layout<Col> {
            lnpro::Alignment { Qt::AlignTop | Qt::AlignHCenter },
            lnpro::Margin { 10 },
            lnpro::SetSpacing { 10 },

            lnpro::Item<Widget> {
                widget::pro::MaximumWidth { 750 },
                widget::pro::Layout {
                    SearchComponent(manager),
                },
            },
            lnpro::Item { BannerComponent(manager) },
            lnpro::Item<Row> {
                lnpro::Item { ItemComponent(manager) },
                lnpro::Item { ItemComponent(manager) },
                lnpro::Item { ItemComponent(manager) },
                lnpro::Item { ItemComponent(manager) },
            },
            lnpro::Item<Row> {
                lnpro::Item { ItemComponent(manager) },
                lnpro::Item { ItemComponent(manager) },
                lnpro::Item { ItemComponent(manager) },
                lnpro::Item { ItemComponent(manager) },
            },
            lnpro::Item<Row> {
                lnpro::Item { ItemComponent(manager) },
                lnpro::Item { ItemComponent(manager) },
                lnpro::Item { ItemComponent(manager) },
                lnpro::Item { ItemComponent(manager) },
            },
        },
    };
}
