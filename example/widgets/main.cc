/// @note
/// 本示例所使用的 ICON 为 Google 提供的 Material Icons Round
/// 字体，为正常显示，需要先下载字体并安装：
/// https://github.com/material-icons/material-icons-font/tree/master/font
/// 如果使用 Arch Linux，则可以通过 AUR 安装：ttf-material-icons-git，
/// 使用其他 Nerd Font 也是可以的

#include "component.hh"
#include <qshortcut.h>

#include <creeper-qt/core/application.hh>
#include <creeper-qt/layout/linear.hh>
#include <creeper-qt/layout/scroll.hh>
#include <creeper-qt/utility/theme/preset/blue-miku.hh>
#include <creeper-qt/utility/theme/theme.hh>
#include <creeper-qt/widget/cards/filled-card.hh>
#include <creeper-qt/widget/main-window.hh>

using namespace creeper;

namespace lnpro = linear::pro;
namespace mwpro = main_window::pro;
namespace capro = card::pro;

auto main(int argc, char** argv) -> int {

    app::init {
        app::pro::Attribute { Qt::AA_EnableHighDpiScaling },
        app::pro::Attribute { Qt::AA_UseHighDpiPixmaps },
        app::pro::Complete { argc, argv },
    };

    auto manager = ThemeManager { kBlueMikuThemePack };

    /// @note 有时候 Windows 总是给我来点惊喜，
    ///       ShowWindow 这么常见命名的函数都放在全局作用域
    creeper::ShowWindow {
        [&](MainWindow& window) {
            // Q 键退出
            auto shortcut_q = new QShortcut { Qt::Key_Q, &window };
            QObject::connect(shortcut_q, &QShortcut::activated, &app::quit);

            // C 键居中
            auto shortcut_c = new QShortcut { Qt::Key_C, &window };
            QObject::connect(shortcut_c, &QShortcut::activated,
                [&window] { window.apply(mwpro::MoveCenter {}); });
        },
        mwpro::MinimumSize { 1080, 720 },
        mwpro::Central<FilledCard> {
            capro::ThemeManager { manager },
            capro::Radius { 0 },
            capro::Level { CardLevel::HIGHEST },

            capro::Layout<Row> {
                lnpro::Margin { 0 },
                lnpro::Spacing { 0 },

                lnpro::Item { NavComponent(manager) },
                lnpro::Item<Col> {
                    lnpro::ContentsMargin { { 15, 15, 5, 15 } },
                    lnpro::Item { ListComponent(manager) },
                },
                lnpro::Item<Col> {
                    { 255 },
                    lnpro::ContentsMargin { { 5, 15, 15, 15 } },
                    lnpro::Item<ScrollArea> {
                        scroll::pro::ThemeManager { manager },
                        scroll::pro::HorizontalScrollBarPolicy {
                            Qt::ScrollBarAlwaysOff,
                        },
                        scroll::pro::Item {
                            ViewComponent(manager),
                        },
                    },
                },
            },
        },
    };

    manager.apply_theme();
    return app::exec();
}
