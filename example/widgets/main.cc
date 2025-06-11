#include <iostream>

#include <qapplication.h>

#include "layout/linear.hh"
#include "widget/button/filled-button.hh"
#include "widget/shape/ellipse.hh"
#include "widget/shape/rounded-rect.hh"
#include "widget/widget.hh"

#include "utility/theme/color-template.hh"
using namespace creeper::util::color;

using namespace creeper;
int main(int argc, char* argv[]) {
    auto application = new QApplication { argc, argv };

    namespace w = widget::pro;
    const auto basic_property
        = std::tuple { w::MinimumSize { 100, 50 }, w::MaximumSize { 500, 300 } };

    namespace rr = rounded_rect::pro;
    namespace fb = filled_button::pro;
    namespace el = ellipse::pro;
    auto widget = new Widget { w::Layout { new Row { linear::pro::Widgets { {
        { new RoundedRect {
            basic_property,
            rr::Background { green200 },
            rr::BorderColor { green300 },
            rr::BorderWidth { 5 },
            rr::Radius { 25 },
        } },
        { new Ellipse {
            el::FixedSize { 100, 100 },
            el::Background { indigo200 },
            el::BorderColor { indigo300 },
            el::BorderWidth { 5 },
        } },
        { new FilledButton {
            basic_property,
            fb::Text { "This Button" },
            fb::Background { green200 },
            fb::BorderColor { green300 },
            fb::WaterColor { green500 },
            fb::Radius { 20 },
            fb::Clickable { [] { std::println(std::cout, "Clicked This Button"); } },
        } },
    } } } } };
    widget->show();

    return application->exec();
}
