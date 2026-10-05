/// 关于图标：
///     本示例所使用的 ICON 为 Google 提供的 Material Icons Round
///     字体，为正常显示，需要先下载字体并安装：
///     https://github.com/material-icons/material-icons-font/tree/master/font
///     如果使用 Arch Linux，则可以通过 AUR 安装：ttf-material-icons-git，
///     使用其他 Nerd Font 也是可以的

#include <creeper-qt/creeper-qt.hh>

#include <QtWidgets>

#include "components/component.hh"
#include "components/display-board.hh"

using namespace creeper;

using namespace linear::pro;
using namespace main_window::pro;
using namespace card::pro;
using namespace stacked::pro;
using namespace scroll::pro;
using namespace mixer::pro;
using namespace app::pro;

auto main(int argc, char** argv) -> int {
    app::init {
        Complete { argc, argv },
    };

    auto final_index = std::uint8_t { 1 };
    auto stack_index = std::make_shared<MutableUInt8>();
    stack_index->set_silent(0);

    auto next_tab = [=] {
        auto is_final = *stack_index == final_index;
        *stack_index  = is_final ? 0 : *stack_index + 1;

        qDebug() << "[nav] Current index: " << *stack_index;
    };

    auto manager = ThemeManager { kBlueMikuThemePack };

    auto nav_component_state = NavComponentState {
        .manager = manager,
        .switch_callback = [&](int index, const auto& name) {
            qDebug() << "[nav] Switch to <" << name.data() << ">";

            constexpr auto kPacks = std::array{
                kBlueMikuThemePack,
                kGreenThemePack,
                kGoldenHarvestThemePack,
            };
            try {
                manager.setThemePack(kPacks.at(index));
            } catch (const std::out_of_range& e) {
                manager.setThemePack(kPacks[0]);
                qDebug() << "[nav] Fallback to kBlueMikuThemePack";
            }
            manager.applyTheme();
        },
        .next_tab = next_tab,
        .buttons_context = {
            {"0", material::icon::kHome},
            {"1", material::icon::kStar},
            {"2", material::icon::kFavorite},
            {"3", material::icon::kExtension},
            {"4", material::icon::kLogout},
        },
    };
    auto list_component_state = ListComponentState { .manager = manager };
    auto view_component_state = ViewComponentState { .manager = manager };

    auto mask_window = (MixerMask*) { };

    /// @NOTE: 有时候 Windows 总是给我来点惊喜，
    ///        ShowWindow 这么常见命名的函数都放在全局作用域
    creeper::ShowWindow<MainWindow> {
        [&](MainWindow& window) noexcept {
            // Q 键退出
            auto shortcut_q = new QShortcut { Qt::Key_Q, &window };
            QObject::connect(shortcut_q, &QShortcut::activated, &app::quit);

            // C 键居中
            auto shortcut_c = new QShortcut { Qt::Key_C, &window };
            QObject::connect(
                shortcut_c, &QShortcut::activated, [&window] { window.use(MoveCenter { }); });

            // S 键保存截图
            auto shortcut_s = new QShortcut { Qt::Key_S, &window };
            QObject::connect(shortcut_s, &QShortcut::activated, [&] {
                const auto pixmap = window.grab();
                const auto format = "yyyy-MM-dd_HH-mm-ss";
                const auto stamp  = QDateTime::currentDateTime().toString(format);

                const auto filename =
                    QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)
                    + QWidget::tr("/main-window-screenshot-%1.png").arg(stamp);

                if (pixmap.save(filename)) {
                    qDebug() << "截图已保存至:" << filename;
                } else {
                    qDebug() << "截图保存失败";
                }
            });
        },
        MinimumSize { 1080, 720 },
        Central<FilledCard> {
          manager,
          Radius { 0 },
          Level { CardLevel::HIGHEST },

          new Row {
            Margin { 0 },
            Spacing { 0 },

            NavComponent(nav_component_state),
            new Col {
              ContentsMargin { 15, 15, 5, 15 },
              ListComponent(list_component_state),
            },
            new Stacked {
              MutableForward {
                CurrentIndex { },
                stack_index,
              },
              new Widget {
                new Col {
                  ContentsMargin { 5, 15, 15, 15 },
                  new ScrollArea {
                    manager,
                    HorizontalScrollBarPolicy {
                      Qt::ScrollBarAlwaysOff,
                    },
                    ScrollItem {
                      ViewComponent(view_component_state),
                    },
                  },
                },
              },
              new Widget {
                new Col {
                  ContentsMargin { { 5, 15, 15, 15 } },
                  new DisplayBoard { manager },
                },
              },
            } + Row::Placement { 1 },
          },
        },
        SetMixerMask { mask_window },
    };

    manager.applyTheme();
    manager.appendBeginCallback([mask_window](const ThemeManager&) {
        // 未 Apply 前，Mask 会呈现灰色
        auto const point = mask_window->mapFromGlobal(QCursor::pos());
        mask_window->initiateAnimation(point);
    });
    return app::exec();
}
