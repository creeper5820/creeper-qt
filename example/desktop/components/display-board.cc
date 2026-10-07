#include "display-board.hh"

#include <creeper-qt/layout/linear.hh>
#include <creeper-qt/layout/stacked.hh>
#include <creeper-qt/utility/material-icon.hh>
#include <creeper-qt/utility/math/lattice.hh>
#include <creeper-qt/utility/wrapper/mutable-value.hh>
#include <creeper-qt/widget/buttons/icon-button.hh>
#include <creeper-qt/widget/custom/widget.hh>
#include <creeper-qt/widget/text.hh>
#include <creeper-qt/widget/widget.hh>

#include <qpainter.h>

#include "components/control-panel.hh"
#include "components/display/checkbox.hh"

namespace creeper {

namespace {

    auto build(ThemeManager& manager) noexcept -> Col* {
        namespace cp  = col::pro;
        namespace rp  = row::pro;
        namespace ibp = icon_button::pro;

        auto index = std::make_shared<MutableValue<int>>(0);
        auto label = std::make_shared<MutableQString>(QString { "1 / 2" });

        const auto step = [index, label](int delta) {
            constexpr auto kCount = 2;

            *index = ((*index + delta) % kCount + kCount) % kCount;
            *label = QString { "%1 / %2" }.arg(*index + 1).arg(kCount);
        };

        auto dot_color =
            std::make_shared<MutableValue<QColor>>(manager.colorScheme().surface_container_highest);
        manager.appendFinalCallback([dot_color](const ThemeManager& m) {
            *dot_color = m.colorScheme().surface_container_highest;
        });

        return new Col {
            new CustomWidget {
              custom::pro::OnPaint {
                std::shared_ptr { dot_color },
                [](CustomWidget& widget, std::shared_ptr<MutableValue<QColor>>& color) {
                    auto solution = LatticeSolution { .spacing = 16 };
                    solution.set_size(widget.size());

                    auto painter = QPainter { &widget };
                    painter.setRenderHint(QPainter::Antialiasing);
                    painter.setPen(Qt::NoPen);
                    painter.setBrush(color->get());
                    for (auto [px, py] : solution.solve()) {
                        painter.drawEllipse(
                            QPointF { static_cast<qreal>(px), static_cast<qreal>(py) }, 1.5, 1.5);
                    }
                },
              },
              MutableTransform { [](CustomWidget&, const QColor&) {}, *dot_color },
              new Col {
                cp::Spacing { 10 },
                cp::Margin { 10 },

                new Widget {
                  new NavHost {
                    MutableForward { nav_host::pro::CurrentIndex { 0 }, index },

                    new CheckboxBoard { manager },
                    ControlPanel(manager),
                  },
                } + Col::Placement { 1 },

                new Row {
                  rp::Alignment { Qt::AlignRight | Qt::AlignVCenter },
                  rp::Spacing { 8 },
                  new IconButton {
                    manager,
                    ibp::FixedSize { 36, 36 },
                    ibp::Color { IconButton::Color::TONAL },
                    ibp::Font { material::kRoundSmallFont },
                    ibp::FontIcon { "chevron_left" },
                    ibp::Clickable { [step] { step(-1); } },
                  },
                  new Text {
                    manager,
                    text::pro::Alignment { Qt::AlignCenter },
                    text::pro::FixedWidth { 64 },
                    MutableForward { text::pro::Text {}, label },
                  },
                  new IconButton {
                    manager,
                    ibp::FixedSize { 36, 36 },
                    ibp::Color { IconButton::Color::TONAL },
                    ibp::Font { material::kRoundSmallFont },
                    ibp::FontIcon { "chevron_right" },
                    ibp::Clickable { [step] { step(1); } },
                  },
                } + Col::Placement { 0, Qt::AlignRight },
              },
            } + Col::Placement { 1 },
        };
    }

} // namespace

DisplayBoard::DisplayBoard(ThemeManager& manager)
    : FilledCard { manager } {
    setLayout(build(manager));
}

}
