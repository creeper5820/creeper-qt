#include "snackbar.hh"

#include <creeper-qt/layout/linear.hh>
#include <creeper-qt/utility/wrapper/foreach.hh>
#include <creeper-qt/utility/wrapper/mutable-value.hh>
#include <creeper-qt/widget/cards/outlined-card.hh>
#include <creeper-qt/widget/snackbar.hh>
#include <creeper-qt/widget/text.hh>

#include <QApplication>
#include <QFocusEvent>
#include <QMouseEvent>

#include <array>

namespace creeper {

namespace {

    constexpr auto kVariantWidth = 480;
    constexpr auto kStateWidth   = 360;

    constexpr auto kLongText = "照片已上传到云端相册，但部分照片因为格式不受支持而被跳过，可以稍后"
                               "在设置中重新尝试上传";

    struct Variant {
        QString title;
        QString text;
        QString action;
        bool dismiss;
        bool new_line;
    };

    enum class StateKind {
        ENABLED,
        ACTION_HOVERED,
        ACTION_FOCUSED,
        ACTION_PRESSED,
        DISMISS_HOVERED,
        DISMISS_PRESSED,
    };

    /// @brief 屏蔽鼠标与键盘交互
    auto BlockInteraction(Snackbar& snackbar) -> void {
        snackbar.setAttribute(Qt::WA_TransparentForMouseEvents, true);
        snackbar.setFocusPolicy(Qt::NoFocus);
    }

    /// @brief 单行布局下按钮触控盒的中心（外边距 12，触控盒 48，短标签 action 宽 58）
    auto DismissCenter(const Snackbar& snackbar) -> QPointF {
        return QPointF { snackbar.width() - 12.0 - 24.0, 36.0 };
    }
    auto ActionCenter(const Snackbar& snackbar) -> QPointF {
        return QPointF { snackbar.width() - 12.0 - 48.0 - 29.0, 36.0 };
    }

    auto SendMouse(Snackbar& snackbar, QEvent::Type type, const QPointF& position) -> void {
        const auto buttons = type == QEvent::MouseMove ? Qt::NoButton : Qt::LeftButton;
        auto event =
            QMouseEvent { type, position, position, Qt::LeftButton, buttons, Qt::NoModifier };
        QApplication::sendEvent(&snackbar, &event);
    }

    auto TitleComponent(ThemeManager& manager, const QString& title) -> Text* {
        return new Text {
            manager,
            text::pro::Text { title },
        };
    }

    auto VariantComponent(ThemeManager& manager, const Variant& variant) -> Col* {
        namespace cp = col::pro;
        namespace sp = snackbar::pro;

        return new Col {
            cp::Spacing { 0 },

            TitleComponent(manager, variant.title),
            new Snackbar {
              manager,
              sp::FixedWidth { kVariantWidth },
              sp::Text { variant.text },
              sp::ActionLabel { variant.action },
              sp::WithDismissAction { variant.dismiss },
              sp::ActionOnNewLine { variant.new_line },
              sp::With { [](Snackbar& self) { BlockInteraction(self); } },
            },
        };
    }

    auto VariantsComponent(ThemeManager& manager) -> Col* {
        namespace cp = col::pro;

        return new Col {
            cp::Spacing { 4 },

            Util::ForEach(
                std::array {
                  Variant { "1 单行", "照片已删除", "", false, false },
                  Variant { "2 单行 + action", "照片已删除", "撤销", false, false },
                  Variant { "3 单行 + dismiss", "无法连接到网络", "", true, false },
                  Variant { "4 单行 + action + dismiss", "照片已删除", "撤销", true, false },
                  Variant { "5 多行", kLongText, "", false, false },
                  Variant { "6 多行 + action + dismiss", kLongText, "重试", true, false },
                  Variant { "7 action 换行", kLongText, "查看详情", false, true },
                  Variant { "8 action 换行 + dismiss", kLongText, "查看详情", true, true },
                },
                [&](const Variant& variant) { return VariantComponent(manager, variant); }),
        };
    }

    auto StateComponent(ThemeManager& manager, const QString& title, StateKind kind) -> Col* {
        namespace cp = col::pro;
        namespace sp = snackbar::pro;

        return new Col {
            cp::Spacing { 0 },

            TitleComponent(manager, title),
            new Snackbar {
              manager,
              sp::FixedWidth { kStateWidth },
              sp::Text { "对话已归档" },
              sp::ActionLabel { "撤销" },
              sp::WithDismissAction { true },
              sp::With { [kind](Snackbar& self) {
                  switch (kind) {
                  case StateKind::ENABLED:
                      break;
                  case StateKind::ACTION_HOVERED:
                      SendMouse(self, QEvent::MouseMove, ActionCenter(self));
                      break;
                  case StateKind::ACTION_FOCUSED: {
                      auto event = QFocusEvent { QEvent::FocusIn, Qt::TabFocusReason };
                      QApplication::sendEvent(&self, &event);
                      break;
                  }
                  case StateKind::ACTION_PRESSED:
                      SendMouse(self, QEvent::MouseButtonPress, ActionCenter(self));
                      break;
                  case StateKind::DISMISS_HOVERED:
                      SendMouse(self, QEvent::MouseMove, DismissCenter(self));
                      break;
                  case StateKind::DISMISS_PRESSED:
                      SendMouse(self, QEvent::MouseButtonPress, DismissCenter(self));
                      break;
                  }
                  BlockInteraction(self);
              } },
            },
        };
    }

    auto StatesComponent(ThemeManager& manager) -> Col* {
        namespace cp = col::pro;

        return new Col {
            cp::Spacing { 4 },

            Util::ForEach(
                std::array {
                  std::tuple { QString { "默认" }, StateKind::ENABLED },
                  std::tuple { QString { "action 悬停" }, StateKind::ACTION_HOVERED },
                  std::tuple { QString { "action 键盘焦点" }, StateKind::ACTION_FOCUSED },
                  std::tuple { QString { "action 按下" }, StateKind::ACTION_PRESSED },
                  std::tuple { QString { "dismiss 悬停" }, StateKind::DISMISS_HOVERED },
                  std::tuple { QString { "dismiss 按下" }, StateKind::DISMISS_PRESSED },
                },
                [&](const auto& spec) {
                    const auto& [title, kind] = spec;
                    return StateComponent(manager, title, kind);
                }),
        };
    }

    /// @brief 交互展示卡片
    auto DemoComponent(ThemeManager& manager) -> OutlinedCard* {
        namespace cp = col::pro;
        namespace sp = snackbar::pro;

        auto message = std::make_shared<MutableQString>(QString { "点击 action 或关闭按钮" });
        auto counts  = std::make_shared<std::array<int, 2>>();

        const auto refresh = [message, counts] {
            *message = QString { "action ×%1，dismiss ×%2" }.arg((*counts)[0]).arg((*counts)[1]);
        };

        return new OutlinedCard {
            outlined_card::pro::SizePolicy { QSizePolicy::Preferred, QSizePolicy::Fixed },
            manager,

            new Col {
              cp::Spacing { 4 },
              cp::Margin { 4 },

              TitleComponent(manager, "交互"),
              new Snackbar {
                manager,
                sp::FixedWidth { kStateWidth },
                MutableForward { sp::Text { }, message },
                sp::ActionLabel { "撤销" },
                sp::WithDismissAction { true },
                sp::OnActionPerformed { [counts, refresh] { ++(*counts)[0], refresh(); } },
                sp::OnDismissed { [counts, refresh] { ++(*counts)[1], refresh(); } },
              },
            },
        };
    }

} // namespace

SnackbarBoard::SnackbarBoard(ThemeManager& manager)
    : Widget { } {
    namespace cp = col::pro;
    namespace rp = row::pro;

    setLayout(new Col {
      cp::Stretch { 1 },

      new Widget {
        new Row {
          rp::Spacing { 24 },
          rp::Alignment { Qt::AlignTop },

          VariantsComponent(manager),
          new Col {
            cp::Spacing { 16 },
            cp::Alignment { Qt::AlignTop },

            StatesComponent(manager),
            DemoComponent(manager),
          },
        },
      } + Col::Placement { 0, Qt::AlignHCenter },

      cp::Stretch { 1 },
    });
}

}
