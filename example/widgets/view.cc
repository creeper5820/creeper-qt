#include <creeper-qt/layout/linear.hh>
#include <creeper-qt/utility/material-icon.hh>
#include <creeper-qt/widget/buttons/icon-button.hh>
#include <creeper-qt/widget/cards/filled-card.hh>
#include <creeper-qt/widget/image.hh>
#include <creeper-qt/widget/text-fields.hh>

using namespace creeper;
namespace capro = card::pro;
namespace lnpro = linear::pro;
namespace impro = image::pro;
namespace tfpro = text_field::pro;
namespace ibpro = icon_button::pro;

static auto SearchComponent(ThemeManager& manager) noexcept {
    return new Row {
        lnpro::Item<FilledTextField> {
            { 255 },
            tfpro::ThemeManager { manager },
            tfpro::MinimumWidth { 400 },
            tfpro::LabelText { "Search" },
            tfpro::FixedHeight { 50 },
            tfpro::LeadingIcon { material::icon::kSearch, material::outlined::font },
        },
        lnpro::Spacing { 40 },
        lnpro::Item<IconButton> {
            ibpro::ThemeManager { manager },
            ibpro::FixedSize { 40, 40 },
            ibpro::Color { IconButton::Color::TONAL },
            ibpro::Font { material::kOutlinedSmallFont },
            ibpro::FontIcon { material::icon::kAdd },
        },
        lnpro::Item<IconButton> {
            ibpro::ThemeManager { manager },
            ibpro::FixedSize { 40, 40 },
            ibpro::Color { IconButton::Color::TONAL },
            ibpro::Font { material::kOutlinedSmallFont },
            ibpro::FontIcon { material::icon::kFavorite },
        },
        lnpro::Item<IconButton> {
            ibpro::ThemeManager { manager },
            ibpro::FixedSize { 40, 40 },
            ibpro::Color { IconButton::Color::TONAL },
            ibpro::Font { material::kOutlinedSmallFont },
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

            col::pro::Item<FilledCard> {
                card::pro::ThemeManager { manager },
                card::pro::Level { CardLevel::LOWEST },
                card::pro::FixedSize { 150, 200 },
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
                lnpro::Alignment { Qt::AlignHCenter },
                lnpro::Item { ItemComponent(manager) },
                lnpro::Item { ItemComponent(manager) },
                lnpro::Item { ItemComponent(manager) },
                lnpro::Item { ItemComponent(manager) },
            },
            lnpro::Item<Row> {
                lnpro::Alignment { Qt::AlignHCenter },
                lnpro::Item { ItemComponent(manager) },
                lnpro::Item { ItemComponent(manager) },
                lnpro::Item { ItemComponent(manager) },
                lnpro::Item { ItemComponent(manager) },
            },
            lnpro::Item<Row> {
                lnpro::Alignment { Qt::AlignHCenter },
                lnpro::Item { ItemComponent(manager) },
                lnpro::Item { ItemComponent(manager) },
                lnpro::Item { ItemComponent(manager) },
                lnpro::Item { ItemComponent(manager) },
            },
        },
    };
}
