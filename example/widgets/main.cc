#include <qapplication.h>
#include <qdebug.h>

#include "layout/linear.hh"
#include "utility/animation/motion-system.hh"
#include "widget/button/filled-button.hh"
#include "widget/button/filled-tonal-button.hh"
#include "widget/button/outlined-button.hh"
#include "widget/button/text-button.hh"
#include "widget/shape/rounded-rect.hh"
#include "widget/widget.hh"

using namespace creeper;

int main(int argc, char* argv[]) {
    // Qt 运行时初始化
    auto application = new QApplication { argc, argv };

    // 创建主题管理器，可以传入主题包
    auto theme_manager = ThemeManager { kBlueMikuThemePack };

    namespace pro = filled_button::pro;
    auto button   = FilledButton {
        pro::ThemeManager { theme_manager }, // 与主题管理器绑定
        pro::FixedSize { 100, 50 },          // 设置固定大小
        pro::Text { "你好世界" },            // 设置文字
        pro::Clickable { [] { qDebug() << "Hello World"; } },
    };
    button.show();

    theme_manager.apply_theme();

    return application->exec();
}

static auto theme_manager = ThemeManager { kBlueMikuThemePack };
int _main(int argc, char* argv[]) {

    auto application = new QApplication { argc, argv };

    auto position        = std::make_shared<Eigen::Vector2d>(0., 540.);
    auto position_target = std::make_shared<Eigen::Vector2d>(0., 540.);

    auto color        = std::make_shared<Eigen::Vector4d>();
    auto color_target = std::make_shared<Eigen::Vector4d>();

    auto stop_token = std::make_shared<bool>(false);
    auto rectangle  = new RoundedRect {
        widget::pro::WindowFlag { Qt::WindowType::ToolTip },
        widget::pro::FixedSize { 100, 100 },
        rounded_rect::pro::Background(from_vector4(*color)),
    };

    rectangle->show();

    using util::animation::AnimationCore;
    auto animatiom_core = AnimationCore { //
        [&rectangle, &position, &color] {
            rectangle->move(position->x(), position->y());
            rectangle->set_background(from_vector4(*color));
            rectangle->update();
        },
        90
    };

    using Tracker2D       = util::animation::InfiniteSringTracker<Eigen::Vector2d>;
    auto position_tracker = Tracker2D { position, position_target, stop_token, 80., 12.0, 90 };
    animatiom_core.append(std::make_unique<Tracker2D>(std::move(position_tracker)));

    using Tracker4D    = util::animation::InfinitePidTracker<Eigen::Vector4d>;
    auto color_tracker = Tracker4D { color, color_target, stop_token, 10., 0., 0., 90 };
    animatiom_core.append(std::make_unique<Tracker4D>(std::move(color_tracker)));

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
                    { new OutlinedButton {
                        properties,
                        button::pro::Text { "Exit" },
                        button::pro::Clickable { [application] { application->exit(); } },
                    } },
                    { new TextButton {
                        properties,
                        button::pro::Text { "200" },
                        button::pro::Clickable { [&position_target, &color_target] {
                            *position_target = Eigen::Vector2d { 100, 100 };
                            *color_target    = from_color(theme_manager.color_scheme().primary);
                        } },
                    } },
                    { new TextButton {
                        properties,
                        button::pro::Text { "300" },
                        button::pro::Clickable { [&position_target, &color_target] {
                            *position_target = Eigen::Vector2d { 200, 100 };
                            *color_target    = from_color(theme_manager.color_scheme().error);
                        } },
                    } },
                } },
                linear::pro::Widget { { new QWidget {}, 1 } },
            } } },
            linear::pro::Widget { { new QWidget {}, 1 } },
        } },
    };
    background_rect->show();

    theme_manager.append_handler(background_rect, [&background_rect](const ThemeManager& manager) {
        const auto color_scheme = manager.color_scheme();
        const auto background   = color_scheme.background;
        background_rect->set_background(background);
        background_rect->update();
    });
    theme_manager.set_color_mode(ColorMode::LIGHT);
    theme_manager.apply_theme();

    return application->exec();
}
