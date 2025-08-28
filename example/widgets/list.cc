#include <creeper-qt/core/application.hh>
#include <creeper-qt/layout/group.hh>
#include <creeper-qt/layout/linear.hh>
#include <creeper-qt/layout/scroll.hh>
#include <creeper-qt/utility/theme/theme.hh>
#include <creeper-qt/widget/buttons/text-button.hh>
#include <creeper-qt/widget/cards/filled-card.hh>
#include <ranges>

using namespace creeper;
namespace lnpro = linear::pro;
namespace grpro = group::pro;
namespace tbpro = text_button::pro;
namespace fcpro = filled_card::pro;

auto ListComponent(ThemeManager& manager) noexcept {

    const auto ButtonGroup = new Group<Col, TextButton> {
        lnpro::Alignment { Qt::AlignTop | Qt::AlignHCenter },
        grpro::Compose {
            std::views::enumerate(std::views::iota('A', 'Z' + 1)),
            [&](int i, char c) {
                return new TextButton {
                    tbpro::ThemeManager { manager },
                    tbpro::FixedWidth { 200 },
                    tbpro::FixedHeight { 50 },
                    tbpro::Radius { -1 },
                    tbpro::Text {
                        std::format("{}.{}{}{}", i, c, c, c),
                    },
                    tbpro::Font { "JetBrains Mono" },
                    tbpro::Clickable {
                        [c](TextButton& button) {
                            auto egg = (c == 'Z') ? "绝区零" : button.text();
                            qDebug() << "[main] Clicked" << egg;
                        },
                    },
                };
            },
            Qt::AlignTop | Qt::AlignHCenter,
        },
    };
    return new FilledCard {
        fcpro::ThemeManager { manager },
        fcpro::Radius { 10 },
        fcpro::Layout<Col> {
            lnpro::Item<ScrollArea> {
                scroll::pro::ThemeManager { manager },
                scroll::pro::ScrollBarPolicy {
                    Qt::ScrollBarAlwaysOff,
                    Qt::ScrollBarAlwaysOff,
                },
                scroll::pro::Item { ButtonGroup },
            },
        },
    };
};
