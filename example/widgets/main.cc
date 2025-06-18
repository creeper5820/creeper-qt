#include <qapplication.h>

#include "layout/linear.hh"
#include "widget/button/filled-button.hh"
#include "widget/button/filled-tonal-button.hh"
#include "widget/button/outlined-button.hh"
#include "widget/button/text-button.hh"
#include "widget/shape/rounded-rect.hh"
#include "widget/switch.hh"
#include "widget/widget.hh"

using namespace creeper;

int main(int argc, char* argv[]) {
    auto application = new QApplication { argc, argv };

    static auto theme_manager = ThemeManager { kBlueMikuThemePack };

    const auto properties = std::tuple {
        util::theme::pro::ThemeManager { theme_manager },
        widget::pro::FixedSize { 100, 50 },
        widget::pro::Font { "JetBrains Mono", 12 },
        button::pro::Text { "你好世界" },
        button::pro::Radius { 25 },
    };

    auto background_rect = (RoundedRect*)nullptr;

    background_rect = new RoundedRect {
        widget::pro::Layout { new Row {
            linear::pro::Widget { { new QWidget {}, 1 } },
            linear::pro::Layout { { new Col {
                linear::pro::Widget { { new QWidget {}, 1 } },
                linear::pro::Widgets { {
                    { new FilledTonalButton {
                        properties,
                        button::pro::Text { "切换主题" },
                        button::pro::Clickable { [] {
                            theme_manager.toggle_color_mode();
                            theme_manager.apply_theme();
                        } },
                    } },
                    { new FilledButton { properties } },
                    { new TextButton { properties } },
                    { new TextButton { properties } },
                    { new Switch {
                        _switch::pro::ThemeManager { theme_manager },
                        _switch::pro::FixedSize { 100, 50 },
                    } },
                    { new OutlinedButton {
                        properties,
                        button::pro::Text { "Exit" },
                        button::pro::Clickable { [application] { application->exit(); } },
                    } },
                } },
                linear::pro::Widget { { new QWidget {}, 1 } },
            } } },
            linear::pro::Widget { { new QWidget {}, 1 } },
        } },
    };
    background_rect->show();

    // 为背景矩形添加主题切换支持
    theme_manager.append_handler(background_rect, //
        [&background_rect](const ThemeManager& manager) {
            const auto color_scheme = manager.color_scheme();
            const auto background   = color_scheme.background;
            background_rect->set_background(background);
            background_rect->update();
        });
    theme_manager.set_color_mode(ColorMode::LIGHT);
    theme_manager.apply_theme();

    return application->exec();
}
