#include "example/widgets/component.hh"

#include <creeper-qt/layout/flow.hh>
#include <creeper-qt/layout/linear.hh>
#include <creeper-qt/utility/material-icon.hh>
#include <creeper-qt/utility/wrapper/mutable.hh>
#include <creeper-qt/widget/buttons/icon-button.hh>
#include <creeper-qt/widget/cards/filled-card.hh>
#include <creeper-qt/widget/image.hh>
#include <creeper-qt/widget/shape/wave-circle.hh>
#include <creeper-qt/widget/sliders.hh>
#include <creeper-qt/widget/text-fields.hh>
#include <creeper-qt/widget/text.hh>

using namespace creeper;
namespace capro = card::pro;
namespace lnpro = linear::pro;
namespace impro = image::pro;
namespace ibpro = icon_button::pro;

namespace repeat_literals {
auto operator*(std::invocable<std::size_t> auto&& f, std::size_t n) {
    std::ranges::for_each(std::views::iota(std::size_t { 0 }, n), std::forward<decltype(f)>(f));
}
auto operator*(std::size_t n, std::invocable<std::size_t> auto&& f) {
    std::ranges::for_each(std::views::iota(std::size_t { 0 }, n), std::forward<decltype(f)>(f));
}
}

static auto SearchComponent(ThemeManager& manager) noexcept {
    const auto row = new Row {
        lnpro::Item<FilledTextField> {
            text_field::pro::ThemeManager { manager },
            text_field::pro::FixedHeight { 50 },
            text_field::pro::LeadingIcon { material::icon::kSearch, material::round::font },
            text_field::pro::LabelText { "你好世界！" },
        },
        lnpro::SpacingItem { 20 },
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
    return new Widget {
        widget::pro::Layout { row },
    };
}
static auto ItemComponent(ThemeManager& manager, int index = 0) noexcept {
    return new Widget {
        widget::pro::Layout<Col> {
            col::pro::Alignment { Qt::AlignTop | Qt::AlignLeft },
            col::pro::Spacing { 5 },
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
                card::pro::LevelLow,
                card::pro::FixedSize { 150, 30 },
                card::pro::Layout<Row> {
                    row::pro::Item<Text> {
                        text::pro::Text { QString { "Item %1" }.arg(index) },
                        text::pro::Apply { [&manager](Text& self) {
                            manager.append_handler(&self, [&](const ThemeManager& manager) {
                                const auto scheme = manager.color_scheme();
                                self.set_color(scheme.primary);
                            });
                        } },
                    },
                },
            },
            col::pro::Item<FilledCard> {
                card::pro::ThemeManager { manager },
                card::pro::LevelLow,
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

static constexpr auto slider_measurements = Slider::Measurements::S();

auto ViewComponent(ViewComponentState& state) noexcept -> raw_pointer<QWidget> {
    const auto SliderComponent = [&] {
        const auto mutable_text = std::make_shared<Mutable<text::pro::Text>>("0.000");

        return new Row {
            lnpro::Alignment { Qt::AlignLeft },
            lnpro::Item<FilledCard> {
                filled_card::pro::ThemeManager { state.manager },
                filled_card::pro::FixedSize { 100, slider_measurements.track_height },
                filled_card::pro::Radius { static_cast<double>(slider_measurements.track_shape) },
                filled_card::pro::LevelLowest,
                filled_card::pro::Layout<Row> {
                    lnpro::Spacing { 0 },
                    lnpro::Margin { 0 },
                    lnpro::Item<Text> {
                        text::pro::ThemeManager { state.manager },
                        text::pro::Alignment { Qt::AlignCenter },
                        text::pro::FixedWidth { 100 },
                        *mutable_text,
                    },
                },
            },
            lnpro::Item<Slider> {
                slider::pro::ThemeManager { state.manager },
                slider::pro::Measurements { slider_measurements },
                slider::pro::FixedHeight { slider_measurements.minimum_height() },
                slider::pro::FixedWidth { 300 },
                slider::pro::OnValueChange {
                    [mutable_text](double progress) mutable {
                        auto string   = QString::number(progress, 'f', 3);
                        *mutable_text = text::pro::Text { string };
                    },
                },
                slider::pro::OnValueChangeFinished {
                    [](double num) { qDebug() << "[view] Slider changed:" << num; } },
            },
        };
    };

    return new FilledCard {
        capro::ThemeManager { state.manager },
        capro::SizePolicy { QSizePolicy::Expanding },
        capro::Layout<Col> {
            lnpro::Alignment { Qt::AlignTop },
            lnpro::Margin { 10 },
            lnpro::Spacing { 10 },

            lnpro::Item { SearchComponent(state.manager) },
            lnpro::Item { BannerComponent(state.manager) },
            lnpro::Item<Row> {
                lnpro::Margin { 20 },
                lnpro::Spacing { 15 },
                lnpro::Item<Col> {
                    lnpro::Item { SliderComponent() },
                    lnpro::Item { SliderComponent() },
                    lnpro::Item { SliderComponent() },
                },
                lnpro::Item<FilledCard> {
                    { 255 },
                    filled_card::pro::ThemeManager { state.manager },
                    filled_card::pro::LevelLowest,
                    filled_card::pro::FixedHeight { slider_measurements.minimum_height() * 3 + 20 },
                },
            },
            lnpro::Item<Flow> {
                flow::pro::RowSpacing { 10 },
                flow::pro::ColSpacing { 10 },
                flow::pro::RowLimit { 6 },
                flow::pro::Apply { [&](Flow& self) {
                    using namespace repeat_literals;
                    1'000 * [&](auto i) { self.addWidget(ItemComponent(state.manager, i)); };
                } },
            },
        },
    };
}
