#include "example/widgets/component.hh"

#include <creeper-qt/core/application.hh>
#include <creeper-qt/layout/group.hh>
#include <creeper-qt/layout/linear.hh>
#include <creeper-qt/layout/scroll.hh>
#include <creeper-qt/utility/theme/theme.hh>
#include <creeper-qt/widget/buttons/text-button.hh>
#include <creeper-qt/widget/cards/filled-card.hh>

#include <qdebug.h>
#include <ranges>

using namespace creeper;
namespace lnpro = linear::pro;
namespace grpro = group::pro;
namespace tbpro = text_button::pro;
namespace fcpro = filled_card::pro;

constexpr auto list_items = std::array {
    "泥土",
    "石头",
    "橡木木板",
    "圆石",
    "沙子",
    "沙砾",
    "玻璃",
    "铁矿石",
    "钻石矿石",
    "岩浆",
    "水",
    "萤石",
    "下界岩",
    "黑曜石",
    "基岩",
    "红石",
    "青金石块",
    "金块",
    "海绵",
    "绿宝石块",
};

auto ListComponent(ListComponentState& state) noexcept -> raw_pointer<QWidget> {

    const auto ButtonGroup = new Group<Col, TextButton> {
        lnpro::Alignment { Qt::AlignTop | Qt::AlignHCenter },
        grpro::Compose {
            std::views::enumerate(list_items),
            [&](int i, auto c) {
                return new TextButton {
                    tbpro::ThemeManager { state.manager },
                    tbpro::FixedWidth { 120 },
                    tbpro::FixedHeight { 30 },
                    tbpro::Radius { -1 },
                    tbpro::Text { std::format("{}.{}", i, c) },
                    tbpro::Font { "JetBrains Mono" },
                    tbpro::Clickable {
                        [c](TextButton& button) { qDebug() << "[main] Clicked" << c; },
                    },
                };
            },
            Qt::AlignTop | Qt::AlignHCenter,
        },
    };
    return new FilledCard {
        fcpro::ThemeManager { state.manager },
        fcpro::Radius { 10 },
        fcpro::Layout<Col> {
            lnpro::Item<ScrollArea> {
                scroll::pro::ThemeManager { state.manager },
                scroll::pro::ScrollBarPolicy {
                    Qt::ScrollBarAlwaysOff,
                    Qt::ScrollBarAlwaysOff,
                },
                scroll::pro::Item { ButtonGroup },
            },
        },
    };
};
