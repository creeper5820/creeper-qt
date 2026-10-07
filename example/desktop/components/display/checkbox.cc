#include "checkbox.hh"

#include <creeper-qt/layout/linear.hh>
#include <creeper-qt/utility/wrapper/foreach.hh>
#include <creeper-qt/utility/wrapper/mutable-value.hh>
#include <creeper-qt/widget/cards/outlined-card.hh>
#include <creeper-qt/widget/checkbox.hh>
#include <creeper-qt/widget/text.hh>

#include <QApplication>
#include <QEnterEvent>
#include <QFocusEvent>
#include <QMouseEvent>
#include <QTimer>

#include <array>

namespace creeper {

namespace {

    enum class CheckboxColumnKind {
        ENABLED,
        DISABLED,
        HOVERED,
        FOCUSED,
        PRESSED,
    };

    /// @brief 屏蔽鼠标与键盘交互
    auto BlockInteraction(Checkbox& box) -> void {
        box.setAttribute(Qt::WA_TransparentForMouseEvents, true);
        box.setFocusPolicy(Qt::NoFocus);
    }

    /// @brief 矩阵单元格
    auto CheckboxCellComponent(
        ThemeManager& manager, Checkbox::CheckState state, CheckboxColumnKind kind, bool error) {

        return new Widget {
            widget::pro::FixedSize { 72, 72 },

            new Col {
              col::pro::Alignment { Qt::AlignCenter },

              new Checkbox {
                manager,
                checkbox::pro::CheckState { state },
                checkbox::pro::Error { error },
                checkbox::pro::With { [kind](Checkbox& self) {
                    BlockInteraction(self);
                    switch (kind) {
                    case CheckboxColumnKind::DISABLED:
                        self.setDisabled(true);
                        break;
                    case CheckboxColumnKind::HOVERED: {
                        auto event =
                            QEnterEvent { QPointF { 1, 1 }, QPointF { 1, 1 }, QPointF { 1, 1 } };
                        QApplication::sendEvent(&self, &event);
                        break;
                    }
                    case CheckboxColumnKind::FOCUSED: {
                        auto event = QFocusEvent { QEvent::FocusIn, Qt::TabFocusReason };
                        QApplication::sendEvent(&self, &event);
                        break;
                    }
                    case CheckboxColumnKind::PRESSED: {
                        auto event = QMouseEvent { QEvent::MouseButtonPress, QPointF { 1, 1 },
                            QPointF { 1, 1 }, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier };
                        QApplication::sendEvent(&self, &event);
                        break;
                    }
                    case CheckboxColumnKind::ENABLED:
                        break;
                    }
                } },
              },
            },
        };
    }

    /// @brief 列头
    auto CheckboxHeaderComponent(ThemeManager& manager, const QString& number) -> Widget* {
        return new Widget {
            widget::pro::FixedSize { 72, 40 },

            new Col {
              col::pro::Alignment { Qt::AlignCenter },

              new Text {
                manager,
                text::pro::Text { number },
                text::pro::Alignment { Qt::AlignCenter },
              },
            },
        };
    }

    /// @brief 矩阵一行
    auto CheckboxRowComponent(ThemeManager& manager, Checkbox::CheckState state, bool error)
        -> Row* {
        namespace rp = row::pro;

        return new Row {
            rp::Alignment { Qt::AlignCenter },

            Util::ForEach(
                std::array {
                  CheckboxColumnKind::ENABLED,
                  CheckboxColumnKind::DISABLED,
                  CheckboxColumnKind::HOVERED,
                  CheckboxColumnKind::FOCUSED,
                  CheckboxColumnKind::PRESSED,
                },
                [&](CheckboxColumnKind kind) {
                    return CheckboxCellComponent(manager, state, kind, error);
                }),
        };
    }

    /// @brief 状态矩阵
    auto CheckboxMatrixComponent(ThemeManager& manager) -> Col* {
        namespace cp = col::pro;
        namespace rp = row::pro;

        return new Col {
            cp::Spacing { 4 },

            new Row {
              rp::Alignment { Qt::AlignCenter },

              Util::ForEach(
                  std::array {
                    QString { "1" },
                    QString { "2" },
                    QString { "3" },
                    QString { "4" },
                    QString { "5" },
                  },
                  [&](const QString& number) { return CheckboxHeaderComponent(manager, number); }),
            },

            Util::ForEach(
                std::array {
                  std::tuple { Checkbox::CheckState::SELECTED, false },
                  std::tuple { Checkbox::CheckState::INDETERMINATE, false },
                  std::tuple { Checkbox::CheckState::UNSELECTED, false },
                  std::tuple { Checkbox::CheckState::SELECTED, true },
                  std::tuple { Checkbox::CheckState::INDETERMINATE, true },
                  std::tuple { Checkbox::CheckState::UNSELECTED, true },
                },
                [&](const auto& spec) {
                    const auto& [state, error] = spec;
                    return CheckboxRowComponent(manager, state, error);
                }),
        };
    }

    /// @brief 交互展示卡片
    auto CheckboxDemoComponent(ThemeManager& manager) -> OutlinedCard* {
        namespace cp = col::pro;

        auto toggle = std::make_shared<MutableBool>(false);
        auto tri =
            std::make_shared<MutableValue<Checkbox::CheckState>>(Checkbox::CheckState::UNSELECTED);
        auto click_toggle = std::make_shared<MutableBool>(false);
        auto click_tri =
            std::make_shared<MutableValue<Checkbox::CheckState>>(Checkbox::CheckState::UNSELECTED);

        return new OutlinedCard {
            outlined_card::pro::SizePolicy { QSizePolicy::Expanding, QSizePolicy::Fixed },
            outlined_card::pro::With {
              [toggle, tri](OutlinedCard& self) {
                  auto* timer = new QTimer { &self };
                  timer->setInterval(1200);
                  QObject::connect(timer, &QTimer::timeout, [toggle, tri] {
                      *toggle = !toggle->get();
                      switch (tri->get()) {
                      case Checkbox::CheckState::SELECTED:
                          *tri = Checkbox::CheckState::INDETERMINATE;
                          break;
                      case Checkbox::CheckState::INDETERMINATE:
                          *tri = Checkbox::CheckState::UNSELECTED;
                          break;
                      case Checkbox::CheckState::UNSELECTED:
                          *tri = Checkbox::CheckState::SELECTED;
                          break;
                      }
                  });
                  timer->start();
              },
            },
            manager,

            new Col {
              cp::Alignment { Qt::AlignVCenter },
              cp::Spacing { 12 },
              cp::Margin { 16 },

              new Text {
                manager,
                text::pro::Text { "定时器驱动" },
                text::pro::Alignment { Qt::AlignCenter },
              } + Col::Placement { 0, Qt::AlignHCenter },
              new Checkbox {
                manager,
                MutableForward { checkbox::pro::Checked { false }, toggle },
                checkbox::pro::With { [](Checkbox& self) { BlockInteraction(self); } },
              } + Col::Placement { 0, Qt::AlignHCenter },
              new Checkbox {
                manager,
                MutableForward {
                  checkbox::pro::CheckState { Checkbox::CheckState::UNSELECTED },
                  tri,
                },
                checkbox::pro::With { [](Checkbox& self) { BlockInteraction(self); } },
              } + Col::Placement { 0, Qt::AlignHCenter },

              new Text {
                manager,
                text::pro::Text { "点击驱动" },
                text::pro::Alignment { Qt::AlignCenter },
              } + Col::Placement { 0, Qt::AlignHCenter },
              new Checkbox {
                manager,
                MutableForward { checkbox::pro::Checked { false }, click_toggle },
                checkbox::pro::Clickable {
                  [click_toggle](auto&) { *click_toggle = !click_toggle->get(); } },
              } + Col::Placement { 0, Qt::AlignHCenter },
              new Checkbox {
                manager,
                MutableForward {
                  checkbox::pro::CheckState { Checkbox::CheckState::UNSELECTED },
                  click_tri,
                },
                checkbox::pro::Clickable { [click_tri](auto&) {
                    switch (click_tri->get()) {
                    case Checkbox::CheckState::SELECTED:
                        *click_tri = Checkbox::CheckState::INDETERMINATE;
                        break;
                    case Checkbox::CheckState::INDETERMINATE:
                        *click_tri = Checkbox::CheckState::UNSELECTED;
                        break;
                    case Checkbox::CheckState::UNSELECTED:
                        *click_tri = Checkbox::CheckState::SELECTED;
                        break;
                    }
                } },
              } + Col::Placement { 0, Qt::AlignHCenter },
            },
        };
    }

} // namespace

CheckboxBoard::CheckboxBoard(ThemeManager& manager)
    : Widget { } {
    namespace cp = col::pro;
    namespace rp = row::pro;

    setLayout(new Col {
      cp::Stretch { 1 },

      new Widget {
        new Row {
          rp::Spacing { 16 },

          CheckboxDemoComponent(manager),
          CheckboxMatrixComponent(manager),
        },
      } + Col::Placement { 0, Qt::AlignHCenter },

      cp::Stretch { 1 },
    });
}

}
