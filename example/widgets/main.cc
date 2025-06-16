#include <qapplication.h>
#include <qdebug.h>
#include <qgraphicseffect.h>

#include <eigen3/Eigen/Dense>

#include "layout/linear.hh"
#include "utility/animation/motion-system.hh"
#include "widget/button/filled-button.hh"
#include "widget/button/filled-tonal-button.hh"
#include "widget/button/outlined-button.hh"
#include "widget/button/text-button.hh"
#include "widget/shape/rounded-rect.hh"
#include "widget/widget.hh"

using namespace creeper;
static auto theme_manager = ThemeManager { kBlueMikuThemePack };

int main(int argc, char* argv[]) {

    auto application = new QApplication { argc, argv };

    auto position   = std::make_shared<Eigen::Vector2d>(0., 540.);
    auto target     = std::make_shared<Eigen::Vector2d>(0., 540.);
    auto stop_token = std::make_shared<bool>(false);
    auto rectangle  = new RoundedRect {
        widget::pro::FixedSize { 100, 100 },
        rounded_rect::pro::Background { theme_manager.color_scheme().primary },
    };
    rectangle->show();

    auto animatiom_core = util::animation::AnimationCore { //
        [&rectangle, &position] {
            rectangle->move(position->x(), position->y());
            qDebug() << position->x() << position->y();
        },
        120
    };
    using Tracker  = util::animation::InfiniteSringTracker<Eigen::Vector2d>;
    auto algorithm = Tracker { position, target, stop_token, 80., 10.0, 120 };
    animatiom_core.append(std::make_unique<Tracker>(std::move(algorithm)));

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
                        button::pro::Clickable {
                            [&target] { *target = Eigen::Vector2d { 100, 100 }; } },
                    } },
                    { new TextButton {
                        properties,
                        button::pro::Text { "300" },
                        button::pro::Clickable {
                            [&target] { *target = Eigen::Vector2d { 1720, 880 }; } },
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
