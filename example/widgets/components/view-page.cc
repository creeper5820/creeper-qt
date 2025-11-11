#include "example/widgets/component.hh"
#include "example/widgets/components/asset-center.hh"

#include <algorithm>
#include <concepts>
#include <creeper-qt/layout/flow.hh>
#include <creeper-qt/layout/linear.hh>
#include <creeper-qt/layout/stacked.hh>
#include <creeper-qt/utility/material-icon.hh>
#include <creeper-qt/utility/wrapper/mutable-value.hh>
#include <creeper-qt/widget/buttons/icon-button.hh>
#include <creeper-qt/widget/cards/filled-card.hh>
#include <creeper-qt/widget/cards/outlined-card.hh>
#include <creeper-qt/widget/image.hh>
#include <creeper-qt/widget/shape/wave-circle.hh>
#include <creeper-qt/widget/sliders.hh>
#include <creeper-qt/widget/switch.hh>
#include <creeper-qt/widget/text-fields.hh>
#include <creeper-qt/widget/text.hh>

#include <cstddef>
#include <qfontdatabase.h>
#include <ranges>
#include <utility>

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
    std::ranges::for_each(std::views::iota(std::size_t(0), n), std::forward<decltype(f)>(f));
}
}

static auto print_material_fonts() noexcept {
    const auto& database = QFontDatabase();
    const auto& families = database.families();
    for (const auto& family : families) {
        if (family.contains("Material", Qt::CaseInsensitive)) {
            qDebug() << "Found Material Font:" << family;
            auto styles = database.styles(family);
            for (const auto& style : styles) {
                qDebug() << " - Style:" << style;
            }
        }
    }
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
    using source_type = std::string_view;
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    const auto sources = std::array {
        source_type( // 大户爱
            "https://c-ssl.duitang.com/uploads/blog/202103/16/20210316112119_181c8.jpeg"),
        source_type( // 长期素食
            "https://c-ssl.duitang.com/uploads/blog/202506/28/V2SOXgQEFm7Q626.jpeg"),
        source_type( // 长期素食
            "https://c-ssl.duitang.com/uploads/blog/202411/25/pGSGeMXVcBx95ZO.jpeg"),
        source_type( // MYGO
            "https://c-ssl.duitang.com/uploads/blog/202507/02/4ESmYpG3Fo8L4mA.jpeg"),
    };

    return new Image {
        impro::ContentScale { ContentScale::CROP },
        impro::SizePolicy { QSizePolicy::Expanding },
        impro::BorderWidth { 3 },
        impro::FixedHeight { 300 },
        impro::PainterResource { sources.at(std::rand() % sources.size()) },
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

auto ViewPageComponent(ViewPageComponentState& state) noexcept -> raw_pointer<QWidget> {

    const auto texts = std::array {
        std::make_shared<MutableValue<QString>>("0.200"),
        std::make_shared<MutableValue<QString>>("0.200"),
        std::make_shared<MutableValue<QString>>("0.200"),
    };
    const auto progresses = std::array {
        std::make_shared<MutableValue<double>>(0.2),
        std::make_shared<MutableValue<double>>(0.2),
        std::make_shared<MutableValue<double>>(0.2),
    };

    const auto SwitchRow = [&] {
        return new Row {
            row::pro::Item<Switch> {
                _switch::pro::ThemeManager { state.manager },
                _switch::pro::FixedSize { 70, 40 },
            },
            row::pro::Item<FilledCard> {
                card::pro::ThemeManager { state.manager },
                card::pro::FixedHeight { 40 },
            },
        };
    };

    const auto SliderComponent = [&](std::shared_ptr<MutableValue<QString>> s,
                                     std::shared_ptr<MutableValue<double>> p) {
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
                        MutableForward { text::pro::Text {}, s },
                    },
                },
            },
            lnpro::Item<Slider> {
                slider::pro::ThemeManager { state.manager },
                slider::pro::Measurements { slider_measurements },
                slider::pro::FixedHeight { slider_measurements.minimum_height() },
                slider::pro::FixedWidth { 300 },
                MutableForward {
                    slider::pro::Progress { 0. },
                    p,
                },
                slider::pro::OnValueChange {
                    [=](double progress) { *s = QString::number(progress, 'f', 3); },
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

            lnpro::Item { BannerComponent(state.manager) },
            lnpro::Item<Row> {
                lnpro::Margin { 20 },
                lnpro::Spacing { 15 },
                lnpro::Item<Col> {
                    lnpro::Item { SliderComponent(texts.at(0), progresses.at(0)) },
                    lnpro::Item { SliderComponent(texts.at(1), progresses.at(1)) },
                    lnpro::Item { SliderComponent(texts.at(2), progresses.at(2)) },
                },
                lnpro::Item<OutlinedCard> {
                    { 255 },
                    card::pro::ThemeManager { state.manager },
                    card::pro::LevelLowest,
                    card::pro::FixedHeight { slider_measurements.minimum_height() * 3 + 40 },
                    card::pro::Layout<Col> {
                        lnpro::Item { SwitchRow() },
                        lnpro::Item { SwitchRow() },
                        lnpro::Item { SwitchRow() },
                    },
                },
            },
            lnpro::Item<AssetCenter> {
                state.manager,
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
