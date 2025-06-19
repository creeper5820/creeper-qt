#include <qapplication.h>
#include <qcolor.h>

#include "layout/linear.hh"
#include "utility/theme/preset/blue-miku.hh"
#include "utility/theme/theme.hh"
#include "widget/button/filled-tonal-button.hh"
#include "widget/button/outlined-button.hh"
#include "widget/button/text-button.hh"
#include "widget/shape/rounded-rect.hh"
#include "widget/switch.hh"
#include "widget/widget.hh"

using namespace creeper;

int main(int argc, char* argv[]) {
    auto application = new QApplication { argc, argv };

    auto theme_manager = ThemeManager { kBlueMikuThemePack };

    auto switch_row = new Row {
        linear::pro::Widget { { new QWidget {}, 1 } },
        linear::pro::Widgets { {
            { new Switch {
                _switch::pro::ThemeManager { theme_manager },
                _switch::pro::FixedSize { 70, 40 },
                _switch::pro::Checked { true },
            } },
            { new Switch {
                _switch::pro::ThemeManager { theme_manager },
                _switch::pro::FixedSize { 70, 40 },
                _switch::pro::Checked { false },
            } },
            { new Switch {
                _switch::pro::ThemeManager { theme_manager },
                _switch::pro::FixedSize { 70, 40 },
                _switch::pro::Checked { true },
                _switch::pro::Disabled { true },
            } },
            { new Switch {
                _switch::pro::ThemeManager { theme_manager },
                _switch::pro::FixedSize { 70, 40 },
                _switch::pro::Checked { false },
                _switch::pro::Disabled { true },
            } },
        } },
        linear::pro::Widget { { new QWidget {}, 1 } },
    };

    auto background_rect = new RoundedRect {
        widget::pro::Layout { new Col {
            linear::pro::Widget { { new QWidget {}, 1 } },
            linear::pro::Layout { { switch_row, 1 } },
            linear::pro::Widget { {
                new Switch {
                    _switch::pro::ThemeManager { theme_manager },
                    _switch::pro::FixedSize { 70, 40 },
                    _switch::pro::Clickable { [&theme_manager](bool checked) {
                        theme_manager.set_color_mode(checked ? ColorMode::DARK : ColorMode::LIGHT);
                        theme_manager.apply_theme();
                    } },
                },
                1,
                Qt::AlignCenter,
            } },
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
