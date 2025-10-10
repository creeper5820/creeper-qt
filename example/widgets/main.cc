/// @note
/// 本示例所使用的 ICON 为 Google 提供的 Material Icons Round
/// 字体，为正常显示，需要先下载字体并安装：
/// https://github.com/material-icons/material-icons-font/tree/master/font
/// 如果使用 Arch Linux，则可以通过 AUR 安装：ttf-material-icons-git，
/// 使用其他 Nerd Font 也是可以的

#include "component.hh"

#include <qdatetime.h>
#include <qshortcut.h>
#include <qstandardpaths.h>

#include <creeper-qt/core/application.hh>
#include <creeper-qt/layout/linear.hh>
#include <creeper-qt/layout/mixer.hh>
#include <creeper-qt/layout/scroll.hh>
#include <creeper-qt/utility/material-icon.hh>
#include <creeper-qt/utility/theme/preset/blue-miku.hh>
#include <creeper-qt/utility/theme/preset/gloden-harvest.hh>
#include <creeper-qt/utility/theme/preset/green.hh>
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

    auto nav_component_state = NavComponentState {
        .manager = manager,
        .switch_callback = [&](int index, const auto& name) {
            qDebug() << "[nav] Switch to <" << name.data() << ">";

            constexpr auto packs = std::array{
                kBlueMikuThemePack,
                kGreenThemePack,
                kGoldenHarvestThemePack,
            };
            try {
                manager.set_theme_pack(packs.at(index));
            } catch (const std::out_of_range& e) {
                manager.set_theme_pack(packs[0]);
                qDebug() << "[nav] Fallback to kBlueMikuThemePack";
            }
            manager.apply_theme();
        },
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

    auto mask_window = (MixerMask*) {};

    /// @note 有时候 Windows 总是给我来点惊喜，
    ///       ShowWindow 这么常见命名的函数都放在全局作用域
    creeper::ShowWindow<MainWindow> {
        [&](MainWindow& window) noexcept {
            // Q 键退出
            auto shortcut_q = new QShortcut { Qt::Key_Q, &window };
            QObject::connect(shortcut_q, &QShortcut::activated, &app::quit);

            // C 键居中
            auto shortcut_c = new QShortcut { Qt::Key_C, &window };
            QObject::connect(shortcut_c, &QShortcut::activated,
                [&window] { window.apply(mwpro::MoveCenter {}); });

            // S 键保存截图
            auto shortcut_s = new QShortcut { Qt::Key_S, &window };
            QObject::connect(shortcut_s, &QShortcut::activated, [&] {
                const auto pixmap = window.grab();
                const auto format = "yyyy-MM-dd_HH-mm-ss";
                const auto stamp  = QDateTime::currentDateTime().toString(format);

                const auto filename =
                    QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)
                    + window.tr("/main-window-screenshot-%1.png").arg(stamp);

                if (pixmap.save(filename)) {
                    qDebug() << "截图已保存至:" << filename;
                } else {
                    qDebug() << "截图保存失败";
                }
            });
        },
        mwpro::MinimumSize { 1080, 720 },
        mwpro::Central<FilledCard> {
            capro::ThemeManager { manager },
            capro::Radius { 0 },
            capro::Level { CardLevel::HIGHEST },

            capro::Layout<Row> {
                lnpro::Margin { 0 },
                lnpro::Spacing { 0 },

                lnpro::Item {
                    NavComponent(nav_component_state),
                },
                lnpro::Item<Col> {
                    lnpro::ContentsMargin { { 15, 15, 5, 15 } },
                    lnpro::Item { ListComponent(list_component_state) },
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
                            ViewComponent(view_component_state),
                        },
                    },
                },
            },
        },
        mixer::pro::SetMixerMask { mask_window },
    };

    manager.apply_theme();
    manager.append_begin_callback([=](const auto&) {
        // 未 Apply 前，Mask 会呈现灰色
        auto const point = mask_window->mapFromGlobal(QCursor::pos());
        mask_window->initiate_animation(point);
    });
    return app::exec();
}
