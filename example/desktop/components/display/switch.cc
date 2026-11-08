#include "switch.hh"

#include <creeper-qt/details/wrapper/foreach.hh>
#include <creeper-qt/details/wrapper/mutable-value.hh>
#include <creeper-qt/layout/linear.hh>
#include <creeper-qt/widget/cards/outlined-card.hh>
#include <creeper-qt/widget/switch.hh>
#include <creeper-qt/widget/text.hh>

#include <QApplication>
#include <QEnterEvent>
#include <QFocusEvent>
#include <QMouseEvent>
#include <QTimer>

#include <array>

namespace creeper {

namespace {

    enum class SwitchColumnKind {
        ENABLED,
        DISABLED,
        HOVERED,
        FOCUSED,
        PRESSED,
    };

    /// @brief 屏蔽鼠标与键盘交互
    auto BlockInteraction(Switch& widget) -> void {
        widget.setAttribute(Qt::WA_TransparentForMouseEvents, true);
        widget.setFocusPolicy(Qt::NoFocus);
    }

    /// @brief 矩阵单元格
    auto SwitchCellComponent(ThemeManager& manager, bool checked, SwitchColumnKind kind)
        -> Widget* {
        return new Widget {
            widget::pro::FixedSize { 84, 72 },

            new Col {
              col::pro::Alignment { Qt::AlignCenter },

              new Switch {
                manager,
                switch_::pro::Checked { checked },
                switch_::pro::With { [kind](Switch& self) {
                    BlockInteraction(self);
                    switch (kind) {
                    case SwitchColumnKind::DISABLED:
                        self.setDisabled(true);
                        break;
                    case SwitchColumnKind::HOVERED: {
                        auto event = QEnterEvent { QPointF { 10, 24 }, QPointF { 10, 24 },
                            QPointF { 10, 24 } };
                        QApplication::sendEvent(&self, &event);
                        break;
                    }
                    case SwitchColumnKind::FOCUSED: {
                        auto event = QFocusEvent { QEvent::FocusIn, Qt::TabFocusReason };
                        QApplication::sendEvent(&self, &event);
                        break;
                    }
                    case SwitchColumnKind::PRESSED: {
                        auto event = QMouseEvent { QEvent::MouseButtonPress, QPointF { 10, 24 },
                            QPointF { 10, 24 }, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier };
                        QApplication::sendEvent(&self, &event);
                        break;
                    }
                    case SwitchColumnKind::ENABLED:
                        break;
                    }
                } },
              },
            },
        };
    }

    /// @brief 列头
    auto SwitchHeaderComponent(ThemeManager& manager, const QString& number) -> Widget* {
        return new Widget {
            widget::pro::FixedSize { 84, 40 },

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
    auto SwitchRowComponent(ThemeManager& manager, bool checked) -> Row* {
        namespace rp = row::pro;

        return new Row {
            rp::Alignment { Qt::AlignCenter },

            Util::ForEach(
                std::array {
                  SwitchColumnKind::ENABLED,
                  SwitchColumnKind::DISABLED,
                  SwitchColumnKind::HOVERED,
                  SwitchColumnKind::FOCUSED,
                  SwitchColumnKind::PRESSED,
                },
                [&](SwitchColumnKind kind) { return SwitchCellComponent(manager, checked, kind); }),
        };
    }

    /// @brief 状态矩阵
    auto SwitchMatrixComponent(ThemeManager& manager) -> Col* {
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
                  [&](const QString& number) { return SwitchHeaderComponent(manager, number); }),
            },

            Util::ForEach(std::array { true, false },
                [&](bool checked) { return SwitchRowComponent(manager, checked); }),
        };
    }

    /// @brief 交互展示卡片
    auto SwitchDemoComponent(ThemeManager& manager) -> OutlinedCard* {
        namespace cp = col::pro;

        auto toggle          = std::make_shared<MutableBool>(false);
        auto click_toggle    = std::make_shared<MutableBool>(false);
        auto disabled_toggle = std::make_shared<MutableBool>(false);

        return new OutlinedCard {
            outlined_card::pro::SizePolicy { QSizePolicy::Expanding, QSizePolicy::Fixed },
            outlined_card::pro::With {
              [toggle](OutlinedCard& self) {
                  auto* timer = new QTimer { &self };
                  timer->setInterval(1200);
                  QObject::connect(timer, &QTimer::timeout, [toggle] { *toggle = !toggle->get(); });
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
              new Switch {
                manager,
                MutableForward { switch_::pro::Checked { false }, toggle },
                switch_::pro::With { [](Switch& self) { BlockInteraction(self); } },
              } + Col::Placement { 0, Qt::AlignHCenter },

              new Text {
                manager,
                text::pro::Text { "点击驱动" },
                text::pro::Alignment { Qt::AlignCenter },
              } + Col::Placement { 0, Qt::AlignHCenter },
              new Switch {
                manager,
                MutableForward { switch_::pro::Checked { false }, click_toggle },
                switch_::pro::Clickable {
                  [click_toggle](auto&) { *click_toggle         = !click_toggle->get(); } },
              } + Col::Placement { 0, Qt::AlignHCenter },
              new Switch {
                manager,
                MutableForward { switch_::pro::Checked { false }, disabled_toggle },
                switch_::pro::Disabled { true },
                switch_::pro::Clickable {
                  [disabled_toggle](Switch&) { *disabled_toggle = !disabled_toggle->get(); } },
              } + Col::Placement { 0, Qt::AlignHCenter },
            },
        };
    }

} // namespace

SwitchBoard::SwitchBoard(ThemeManager& manager)
    : Widget { } {
    namespace cp = col::pro;
    namespace rp = row::pro;

    setLayout(new Col {
      cp::Stretch { 1 },

      new Widget {
        new Row {
          rp::Spacing { 16 },

          SwitchDemoComponent(manager),
          SwitchMatrixComponent(manager),
        },
      } + Col::Placement { 0, Qt::AlignHCenter },

      cp::Stretch { 1 },
    });
}

}
