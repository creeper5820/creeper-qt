#include <qapplication.h>
#include <qdebug.h>
#include <qgraphicseffect.h>

#include "layout/linear.hh"
#include "widget/button/filled-button.hh"
#include "widget/button/filled-tonal-button.hh"
#include "widget/shape/rounded-rect.hh"
#include "widget/widget.hh"

void foo() {
    namespace pro = creeper::filled_button::pro;
    using creeper::FilledButton;

    auto filled_button = new FilledButton {
        pro::FixedSize { 100, 50 },
        pro::Text { "Hello World" },
        pro::Font { "JetBrains Mono", 12 },
        pro::Radius { 25 },
    };
}

using namespace creeper;
namespace w   = widget::pro;
namespace fb  = filled_button::pro;
namespace ftb = filled_tonal_button::pro;
int main(int argc, char* argv[]) {

    auto application     = new QApplication { argc, argv };
    auto theme_manager   = ThemeManager {};
    auto background_rect = (RoundedRect*)nullptr;

    const auto ftb_properties = std::tuple {
        ftb::ThemeManager { theme_manager },
        ftb::FixedSize { 100, 50 },
        ftb::Text { "你好世界" },
        ftb::Font { "JetBrains Mono", 12 },
        ftb::Radius { 25 },
    };
    const auto fb_properties = std::tuple {
        fb::ThemeManager { theme_manager },
        fb::FixedSize { 100, 50 },
        fb::Text { "你好世界" },
        fb::Font { "JetBrains Mono", 12 },
        fb::Radius { 25 },
    };

    const auto switch_theme = [&theme_manager, &background_rect](ColorMode mode) {
        theme_manager.set_color_mode(mode);
        theme_manager.apply_theme();

        const auto color_scheme = theme_manager.color_scheme();
        const auto background   = color_scheme.background;

        background_rect->set_background(background);
        background_rect->update();
    };

    background_rect = new RoundedRect {
        w::Layout { new Col {
            linear::pro::Widget { { new QWidget {}, 1 } },
            linear::pro::Widgets { {
                { new FilledTonalButton {
                      ftb_properties,
                      ftb::Text { "明亮主题" },
                      ftb::Clickable { [switch_theme] { switch_theme(ColorMode::LIGHT); } },
                  },
                    0, Qt::AlignCenter },
                { new FilledTonalButton {
                      ftb_properties,
                      ftb::Text { "黑暗主题" },
                      ftb::Clickable { [switch_theme] { switch_theme(ColorMode::DARK); } },
                  },
                    0, Qt::AlignCenter },
                { new FilledButton { fb_properties }, 0, Qt::AlignCenter },
                { new FilledButton { fb_properties }, 0, Qt::AlignCenter },
            } },
            linear::pro::Widget { { new QWidget {}, 1 } },
        } },
    };
    background_rect->show();

    theme_manager.set_theme_pack(kBlueMikuThemePack);
    switch_theme(ColorMode::DARK);

    return application->exec();
}
