#include <qapplication.h>
#include <qcolor.h>

#include "creeper-qt/layout/linear.hh"
#include "creeper-qt/utility/theme/preset/blue-miku.hh"
#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/widget/button/filled-tonal-button.hh"
#include "creeper-qt/widget/button/outlined-button.hh"
#include "creeper-qt/widget/button/text-button.hh"
#include "creeper-qt/widget/shape/rounded-rect.hh"
#include "creeper-qt/widget/switch.hh"
#include "creeper-qt/widget/widget.hh"

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

    namespace widget = widget::pro;
    namespace button = button::pro;
    namespace linear = linear::pro;

    namespace pro = filled_button::pro;

    const auto button_pro = std::tuple {
        pro::ThemeManager { theme_manager },
        pro::FixedSize { 110, 50 },
        pro::Text { "你好世界" },
        pro::Font { "WenQuanYi Zen Hei", 13 },
        pro::Radius { 25 },
    };

    const auto button_row = new Row {
        linear::Widget { { new Widget, 1 } },
        linear::Widgets { {
            { new FilledButton {
                button_pro,
                button::Text { "威严满满" },
                button::Clickable { [] { //
                    qDebug() << "抱头蹲防";
                } },
            } },
            { new FilledTonalButton {
                button_pro,
                button::Text { "Color Mode" },
                button::Clickable { [&theme_manager] {
                    theme_manager.toggle_color_mode();
                    theme_manager.apply_theme();
                } },
            } },
            { new OutlinedButton { button_pro } },
            { new TextButton {
                button_pro,
                button::Text { "退出应用" },
                button::Clickable { [&application] { //
                    application->exit();
                } },
            } },
        } },
        linear::Widget { { new Widget, 1 } },
    };

    auto background_rect = new RoundedRect {
        widget::WindowFlag { Qt::WindowType::SplashScreen },
        widget::FixedSize { 500, 300 },
        widget::Layout { new Col {
            linear::Widget { { new Widget, 1 } },
            linear::Layout { { button_row, 1 } },
            linear::Widget { { new Widget, 1 } },
        } },
    };
    background_rect->show();
    background_rect->move(960 - background_rect->width() / 2, 540 - background_rect->height() / 2);

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
