#include "creeper-qt/creeper-qt.hh"
#include <QtWidgets/QtWidgets>

using namespace creeper;

namespace pre {
namespace internal {
    auto manager = ThemeManager {
        kBlueMikuThemePack,
    };
}
namespace pro {
    auto theme = util::theme::pro::ThemeManager {
        internal::manager,
    };
    auto font = widget::pro::Font { "WenQuanYi Micro Hei", 10 };
}
namespace lambda {
    const auto change_color = [] {
        internal::manager.toggle_color_mode();
        internal::manager.apply_theme();
    };
}
}

int main(int argc, char* argv[]) {
    app::init {
        app::pro::Attribute { Qt::AA_EnableHighDpiScaling },
        app::pro::Attribute { Qt::AA_UseHighDpiPixmaps },
        app::pro::Complete { argc, argv },
    };

    const auto workspace_navigation = new FilledCard {
        pre::pro::theme,
        card::pro::Radius { 0 },

        card::pro::Layout<Col> {
            linear::pro::Stretch { 255 },
            linear::pro::Item<OutlinedButton> {
                pre::pro::theme,
                pre::pro::font,
                button::pro::FixedSize { 70, 40 },
                button::pro::Text { "颜色模式" },
                button::pro::Radius { -1 },
                button::pro::BorderWidth { 0.5 },
                button::pro::Clickable { pre::lambda::change_color },
            },
        },
    };

    const auto background = new FilledCard {
        pre::pro::theme,
        card::pro::MinimumSize { 720, 480 },
        card::pro::Radius { 0 },
        card::pro::Level { CardLevel::HIGHEST },

        card::pro::Layout<Row> {
            linear::pro::Margin { 0 },
            linear::pro::SetSpacing { 5 },

            linear::pro::Item { { 1, Qt::AlignLeft }, workspace_navigation },
            linear::pro::Stretch { 5 },
        },
    };

    auto window = new QMainWindow {};
    window->setCentralWidget(background);
    window->show();

    pre::internal::manager.apply_theme();
    return app::exec();
}
