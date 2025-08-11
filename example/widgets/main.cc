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
    auto font = widget::pro::Font { "WenQuanYi Micro Hei", 12 };

    const auto outlined_button_meta = std::tuple {
        pre::pro::theme,
        pre::pro::font,
        button::pro::FixedSize { 100, 50 },
        button::pro::Radius { -1 },
        button::pro::BorderWidth { 0.5 },
    };
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
                pre::pro::outlined_button_meta,
                button::pro::Text { "退出程序" },
                button::pro::Clickable {
                    [] { QApplication::quit(); },
                },
            },
            linear::pro::Item<OutlinedButton> {
                pre::pro::outlined_button_meta,
                button::pro::Text { "颜色模式" },
                button::pro::Clickable { pre::lambda::change_color },
            },
        },
    };

    auto image = (Image*) {};

    const auto background = new FilledCard {
        pre::pro::theme,
        card::pro::MinimumSize { 720, 480 },
        card::pro::Radius { 0 },
        card::pro::Level { CardLevel::HIGHEST },

        card::pro::Layout<Row> {
            linear::pro::Margin { 0 },
            linear::pro::SetSpacing { 5 },

            linear::pro::Item { workspace_navigation },
            linear::pro::Item<FilledCard> {
                { 255 },
                card::pro::Layout<Col> {
                    linear::pro::Item<Image> {
                        image::pro::PainterResource {
                            "https://c-ssl.duitang.com/uploads/blog/202104/08/"
                            "20210408220918_a9e0d.jpg",
                            [&] { image->update(); },
                        },
                        image::pro::Bind { image },
                        image::pro::ContentScale { ContentScale::CROP },
                    },
                },
            },
        },
    };

    auto window = new QMainWindow {};
    window->setCentralWidget(background);
    window->show();

    pre::internal::manager.apply_theme();
    return app::exec();
}
