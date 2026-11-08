#include "display-board.hh"

#include <creeper-qt/details/wrapper/mutable-value.hh>
#include <creeper-qt/layout/linear.hh>
#include <creeper-qt/layout/scroll.hh>
#include <creeper-qt/layout/stacked.hh>
#include <creeper-qt/utility/material-icon.hh>
#include <creeper-qt/utility/math/lattice.hh>
#include <creeper-qt/widget/buttons/icon-button.hh>
#include <creeper-qt/widget/custom/widget.hh>
#include <creeper-qt/widget/text.hh>
#include <creeper-qt/widget/widget.hh>

#include <qpainter.h>

#include "components/display/checkbox.hh"
#include "components/display/snackbar.hh"
#include "components/display/switch.hh"

namespace creeper {

struct DisplayBoard::Impl {
    static constexpr auto kCount = 3;

    MutableValue<int> index { 0 };
    MutableQString label { QString { "1 / 3" } };
    MutableValue<QColor> dot_color { QColor { } };

    auto step(int delta) -> void {
        index = ((index.get() + delta) % kCount + kCount) % kCount;
        label = QString { "%1 / %2" }.arg(index.get() + 1).arg(kCount);
    }

    auto build(ThemeManager& manager) noexcept -> Col* {
        namespace cp  = col::pro;
        namespace rp  = row::pro;
        namespace ibp = icon_button::pro;
        namespace sp  = scroll::pro;

        dot_color.set_silent(manager.colorScheme().surface_container_highest);

        auto* dot_widget = new CustomWidget {
            custom::pro::OnPaint {
              [this](CustomWidget& widget) {
                  auto solution = LatticeSolution { .spacing = 16 };
                  solution.set_size(widget.size());

                  auto painter = QPainter { &widget };
                  painter.setRenderHint(QPainter::Antialiasing);
                  painter.setPen(Qt::NoPen);
                  painter.setBrush(dot_color.get());
                  for (auto [point_x, point_y] : solution.solve()) {
                      painter.drawEllipse(
                          QPointF { static_cast<qreal>(point_x), static_cast<qreal>(point_y) }, 1.5,
                          1.5);
                  }
              },
            },
            MutableTransform { [](CustomWidget&, const QColor&) { }, dot_color },
            new Col {
              cp::Spacing { 10 },
              cp::Margin { 10 },

              new Widget {
                new NavHost {
                  MutableForward { nav_host::pro::CurrentIndex { 0 }, index },

                  new ScrollArea {
                    manager,
                    sp::ScrollBarPolicy { Qt::ScrollBarAsNeeded, Qt::ScrollBarAsNeeded },
                    sp::ScrollItem<CheckboxBoard> { manager },
                  },
                  new ScrollArea {
                    manager,
                    sp::ScrollBarPolicy { Qt::ScrollBarAsNeeded, Qt::ScrollBarAsNeeded },
                    sp::ScrollItem<SnackbarBoard> { manager },
                  },
                  new ScrollArea {
                    manager,
                    sp::ScrollBarPolicy { Qt::ScrollBarAsNeeded, Qt::ScrollBarAsNeeded },
                    sp::ScrollItem<SwitchBoard> { manager },
                  },
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
                  ibp::Clickable { [this] { step(-1); } },
                },
                new Text {
                  manager,
                  text::pro::Alignment { Qt::AlignCenter },
                  text::pro::FixedWidth { 64 },
                  MutableForward { text::pro::Text { }, label },
                },
                new IconButton {
                  manager,
                  ibp::FixedSize { 36, 36 },
                  ibp::Color { IconButton::Color::TONAL },
                  ibp::Font { material::kRoundSmallFont },
                  ibp::FontIcon { "chevron_right" },
                  ibp::Clickable { [this] { step(1); } },
                },
              } + Col::Placement { 0, Qt::AlignRight },
            },
        };

        manager.appendHandler(dot_widget, [this](const ThemeManager& theme_manager) {
            dot_color = theme_manager.colorScheme().surface_container_highest;
        });

        return new Col {
            dot_widget + Col::Placement { 1 },
        };
    }
};

DisplayBoard::DisplayBoard()
    : pimpl(std::make_unique<Impl>()) { }

DisplayBoard::DisplayBoard(ThemeManager& manager)
    : DisplayBoard { } {
    bindThemeManager(manager);
    setLayout(pimpl->build(manager));
}

DisplayBoard::~DisplayBoard() = default;

}
