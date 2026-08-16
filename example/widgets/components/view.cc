#include "component.hh"
#include "components/asset-center.hh"

#include <creeper-qt/layout/flow.hh>
#include <creeper-qt/layout/linear.hh>
#include <creeper-qt/layout/stacked.hh>
#include <creeper-qt/utility/material-icon.hh>
#include <creeper-qt/utility/math/lattice.hh>
#include <creeper-qt/utility/wrapper/foreach.hh>
#include <creeper-qt/utility/wrapper/mutable-value.hh>
#include <creeper-qt/widget/buttons/filled-button.hh>
#include <creeper-qt/widget/buttons/icon-button.hh>
#include <creeper-qt/widget/cards/filled-card.hh>
#include <creeper-qt/widget/cards/outlined-card.hh>
#include <creeper-qt/widget/custom/widget.hh>
#include <creeper-qt/widget/dropdown-menu-item.hh>
#include <creeper-qt/widget/dropdown-menu.hh>
#include <creeper-qt/widget/image.hh>
#include <creeper-qt/widget/indicator/circular-progress-indicator.hh>
#include <creeper-qt/widget/shape/wave-circle.hh>
#include <creeper-qt/widget/sliders.hh>
#include <creeper-qt/widget/switch.hh>
#include <creeper-qt/widget/text-fields.hh>
#include <creeper-qt/widget/text.hh>

#include <chrono>
#include <qfontdatabase.h>
#include <qtimer.h>
#include <random>

using namespace creeper;

namespace cap  = card::pro;
namespace lnp  = linear::pro;
namespace imp  = image::pro;
namespace ibp  = icon_button::pro;
namespace cpip = circular_progress_indicator::pro;
namespace dmp  = dropdown_menu::pro;
namespace dmip = dropdown_menu_item::pro;
namespace tfp  = text_field::pro;

namespace repeat_literals {
auto operator*(std::invocable<std::size_t> auto&& f, std::size_t n) {
    std::ranges::for_each(std::views::iota(std::size_t { 0 }, n), std::forward<decltype(f)>(f));
}
auto operator*(std::size_t n, std::invocable<std::size_t> auto&& f) {
    std::ranges::for_each(std::views::iota(std::size_t { 0 }, n), std::forward<decltype(f)>(f));
}
} // namespace repeat_literals

static auto print_material_fonts() noexcept {
    const auto& families = QFontDatabase::families();
    for (const auto& family : families) {
        if (family.contains("Material", Qt::CaseInsensitive)) {
            qDebug() << "Found Material Font:" << family;
            auto styles = QFontDatabase::styles(family);
            for (const auto& style : styles) {
                qDebug() << " - Style:" << style;
            }
        }
    }
}

static auto SearchComponent(ThemeManager& manager, auto&& refresh_callback) noexcept {

    struct Context {
        std::chrono::steady_clock::time_point timeline = std::chrono::steady_clock::now();

        MutableQString slogen { "BanG Dream! It’s MyGO!!!!!" };
        MutableQString selected { };
        MutableBool menu_expanded { false };
        MutableBool loading { false };
        MutableDouble progress { 0.0 };
    };
    const auto context = std::make_shared<Context>();

    const auto row = new Row {
        lnp::Item<OutlinedTextField> {
            MutableForward {
                tfp::LabelText { },
                context->slogen,
            },
            tfp::ThemeManager { manager },
            tfp::LeadingIcon {
                material::icon::kSearch,
                material::round::font,
            },
            tfp::Measurements {
                OutlinedTextField::Measurements { },
            },
            tfp::OnChanged {
                [context](const QString& text) {
                    const auto count = text.size();

                    using namespace std::chrono_literals;
                    constexpr auto kCheckInterval = 1s;

                    context->timeline = std::chrono::steady_clock::now() + kCheckInterval;
                    context->loading  = true;
                    QTimer::singleShot(kCheckInterval, [context, count] {
                        if (std::chrono::steady_clock::now() < context->timeline) return;
                        context->loading  = false;
                        context->progress = std::min(count / 20.0, 1.0);
                    });
                },
            },
        },

        lnp::SpacingItem { 10 },

        lnp::Item<OutlinedTextField> {
            MutableForward { tfp::Text { }, context->selected },

            tfp::ThemeManager { manager },
            tfp::ReadOnly { true },
            tfp::LabelText { "成员" },
            tfp::FixedWidth { 120 },

        },
        lnp::Item<IconButton> {
            ibp::ThemeManager { manager },
            ibp::FixedSize { 40, 40 },
            ibp::Color { IconButton::Color::TONAL },
            ibp::Font { material::kRoundSmallFont },
            ibp::FontIcon { material::icon::kArrowDropDown },
            ibp::Clickable { [context] { context->menu_expanded = true; } },

            ibp::Child<DropdownMenu> {
                MutableForward { dmp::Expanded { false }, context->menu_expanded },

                dmp::ThemeManager { manager },
                dmp::OnDismissRequest { [context] { context->menu_expanded = false; } },

                Util::ForEach(std::array { "高松灯", "千早爱音", "要乐奈", "长崎爽世", "椎名立希" },
                    [&](auto index, auto name) {
                        return dmp::Item<DropdownMenuItem> {
                            dmip::ThemeManager { manager },
                            dmip::Text { std::format("{}. {}", index + 1, name) },
                            dmip::OnClicked { [=] {
                                context->selected      = name;
                                context->menu_expanded = false;

                                qDebug() << "[view] Select " << name;
                            } },
                        };
                    }),
            },

        },

        lnp::SpacingItem { 10 },

        lnp::Item<IconButton> {
            ibp::ThemeManager { manager },
            ibp::FixedSize { 40, 40 },
            ibp::Color { IconButton::Color::TONAL },
            ibp::Font { material::kRoundSmallFont },
            ibp::FontIcon { "change_circle" },
            ibp::Clickable { refresh_callback },
        },
        lnp::Item<IconButton> {
            ibp::ThemeManager { manager },
            ibp::FixedSize { 40, 40 },
            ibp::Color { IconButton::Color::TONAL },
            ibp::Font { material::kRoundSmallFont },
            ibp::FontIcon { material::icon::kFavorite },
            ibp::Clickable { [context] {
                constexpr auto random_slogen = [] {
                    constexpr auto slogens = std::array {
                        "为什么要演奏《春日影》！",
                        "我从来不觉得玩乐队开心过。",
                        "我好想…成为人啊！",
                        "那你愿意……跟我组一辈子的乐队吗？",
                        "过去软弱的我…已经死了。",
                    };
                    static std::random_device rd;
                    static std::mt19937 gen(rd());
                    std::uniform_int_distribution<> dist(0, slogens.size() - 1);
                    return QString::fromUtf8(slogens[dist(gen)]);
                };
                context->slogen = random_slogen();
            } },
        },
        lnp::Item<IconButton> {
            ibp::ThemeManager { manager },
            ibp::FixedSize { 40, 40 },
            ibp::Color { IconButton::Color::TONAL },
            ibp::Font { material::kRoundSmallFont },
            ibp::FontIcon { "font_download" },
            ibp::Clickable { &print_material_fonts },
        },

        lnp::SpacingItem { 10 },

        lnp::Item<CircularProgressIndicator> {
            MutableForward { cpip::Indeterminate { false }, context->loading },
            MutableForward { cpip::Progress { 0. }, context->progress },
            cpip::ThemeManager { manager },
            cpip::FixedSize { 40, 40 },
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
static auto DropdownMenuItemComponent(ThemeManager& manager) noexcept {

    return new Widget {
        widget::pro::FixedSize { 170, 290 },
        widget::pro::Layout<Col> {
            col::pro::Alignment { Qt::AlignVCenter | Qt::AlignHCenter },
            col::pro::Spacing { 0 },
            col::pro::Margin { 10 },

            col::pro::Item<DropdownMenuItem> {
                dmip::ThemeManager { manager },
                dmip::Text { "编辑" },
                dmip::LeadingIcon {
                    material::icon::kEdit,
                    material::round::font,
                },
                dmip::TrailingIcon {
                    material::icon::kArrowRight,
                    material::round::font,
                },
                dmip::OnClicked { [] { qDebug() << "[view] 编辑"; } },
            },
            col::pro::Item<DropdownMenuItem> {
                dmip::ThemeManager { manager },
                dmip::Text { "收藏" },
                dmip::LeadingIcon {
                    material::icon::kFavorite,
                    material::round::font,
                },
                dmip::OnClicked { [] { qDebug() << "[view] 收藏"; } },
            },
            col::pro::Item<DropdownMenuItem> {
                dmip::ThemeManager { manager },
                dmip::Text { "删除（禁用）" },
                dmip::Disabled { true },
                dmip::OnClicked { [] { qDebug() << "[view] 删除"; } },
            },
        },
    };
}
static auto BannerComponent(ThemeManager& manager) noexcept {
    using source_type = std::string_view;
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    const auto sources = std::array {
        source_type( // 大户爱
            "https://c-ssl.duitang.com/uploads/blog/202103/16/"
            "20210316112119_181c8.jpeg"),
        source_type( // 长期素食
            "https://c-ssl.duitang.com/uploads/blog/202506/28/"
            "V2SOXgQEFm7Q626.jpeg"),
        source_type( // 长期素食
            "https://c-ssl.duitang.com/uploads/blog/202411/25/"
            "pGSGeMXVcBx95ZO.jpeg"),
        source_type( // MYGO
            "https://c-ssl.duitang.com/uploads/blog/202507/02/"
            "4ESmYpG3Fo8L4mA.jpeg"),
    };

    return new Image {
        imp::ContentScale { ContentScale::CROP },
        imp::SizePolicy { QSizePolicy::Expanding },
        imp::BorderWidth { 3 },
        imp::FixedHeight { 300 },
        imp::PainterResource { sources.at(std::rand() % sources.size()) },
        imp::Apply { [&manager](Image& self) {
            manager.append_handler(&self, [&](const ThemeManager& manager) {
                const auto colorscheme = manager.color_scheme();
                const auto colorborder = colorscheme.secondary_container;
                self.set_border_color(colorborder);
            });
        } },
    };
}

constexpr auto kSliderMeasurements = Slider::Measurements::S();

auto ViewComponent(ViewComponentState& state) noexcept -> raw_pointer<QWidget> {

    const auto texts = std::array {
        std::make_shared<MutableValue<QString>>("0.500"),
        std::make_shared<MutableValue<QString>>("0.500"),
        std::make_shared<MutableValue<QString>>("0.500"),
    };
    const auto progresses = std::array {
        std::make_shared<MutableValue<double>>(0.5),
        std::make_shared<MutableValue<double>>(0.5),
        std::make_shared<MutableValue<double>>(0.5),
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
            lnp::Alignment { Qt::AlignLeft },
            lnp::Item<FilledCard> {
                filled_card::pro::ThemeManager { state.manager },
                filled_card::pro::FixedSize { 100, kSliderMeasurements.track_height },
                filled_card::pro::Radius { static_cast<double>(kSliderMeasurements.track_shape) },
                filled_card::pro::LevelLowest,
                filled_card::pro::Layout<Row> {
                    lnp::Spacing { 0 },
                    lnp::Margin { 0 },
                    lnp::Item<Text> {
                        text::pro::ThemeManager { state.manager },
                        text::pro::Alignment { Qt::AlignCenter },
                        text::pro::FixedWidth { 100 },
                        MutableForward { text::pro::Text { }, s },
                    },
                },
            },
            lnp::Item<Slider> {
                slider::pro::ThemeManager { state.manager },
                slider::pro::Measurements { kSliderMeasurements },
                slider::pro::FixedHeight { kSliderMeasurements.minimum_height() },
                slider::pro::FixedWidth { 300 },
                MutableForward {
                    slider::pro::Progress { 0. },
                    p,
                },
                slider::pro::OnValueChange {
                    [=](double progress) {
                        *s = QString::number(progress, 'f', 3);
                        *p = progress;
                    },
                },
                slider::pro::OnValueChangeFinished {
                    [](double num) { qDebug() << "[view] Slider changed:" << num; } },
            },
            lnp::Item<CircularProgressIndicator> {
                cpip::ThemeManager { state.manager },
                cpip::FixedSize { 25, 25 },
                MutableForward { cpip::Progress { 0. }, p },
            },
        };
    };

    return new FilledCard {
        cap::ThemeManager { state.manager },
        cap::SizePolicy { QSizePolicy::Expanding },

        cap::Layout<Col> {
            lnp::Alignment { Qt::AlignTop },
            lnp::Margin { 10 },
            lnp::Spacing { 10 },

            lnp::Item {
                SearchComponent(state.manager,
                    [texts, progresses] {
                        constexpr auto random_unit = []() {
                            static std::random_device rd;
                            static std::mt19937 gen(rd());
                            static std::uniform_real_distribution<double> dist(0.0, 1.0);
                            return dist(gen);
                        };
                        for (auto&& [string, number] : std::views::zip(texts, progresses)) {
                            auto v  = random_unit();
                            *number = v;
                            *string = QString::number(v, 'f', 3);
                        }
                    }),
            },
            lnp::Item { BannerComponent(state.manager) },
            lnp::Item<Row> {
                lnp::Margin { 20 },
                lnp::Spacing { 15 },
                lnp::Item<Col> {
                    lnp::Item { SliderComponent(texts.at(0), progresses.at(0)) },
                    lnp::Item { SliderComponent(texts.at(1), progresses.at(1)) },
                    lnp::Item { SliderComponent(texts.at(2), progresses.at(2)) },
                },
                lnp::Item<OutlinedCard> {
                    { 255 },
                    card::pro::ThemeManager { state.manager },
                    card::pro::LevelLowest,
                    card::pro::FixedHeight { kSliderMeasurements.minimum_height() * 3 + 40 },
                    card::pro::Layout<Col> {
                        lnp::Item { SwitchRow() },
                        lnp::Item { SwitchRow() },
                        lnp::Item { SwitchRow() },
                    },
                },
            },
            lnp::Item<AssetCenter> {
                state.manager,
            },
            lnp::Item<CustomWidget> {
                custom::pro::FixedHeight { 300 },
                custom::pro::OnPaint<std::reference_wrapper<ThemeManager>> {
                    std::ref(state.manager),
                    [](auto& widget, ThemeManager& state) {
                        auto solution = LatticeSolution {
                            .spacing = 15,
                        };
                        solution.set_size(widget.size());

                        auto painter = QPainter { &widget };
                        painter.setBrush(state.color_scheme().surface_container_highest);
                        painter.setPen(Qt::NoPen);
                        painter.setRenderHint(QPainter::Antialiasing);
                        for (auto [px, py] : solution.solve()) {
                            painter.drawEllipse(px, py, 4, 4);
                        }
                    },
                },
            },
            lnp::Item<Flow> {
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
