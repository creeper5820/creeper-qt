#include <iostream>
#include <qapplication.h>

#include "layout/linear.hh"
#include "widget/button/filled-button.hh"
#include "widget/shape/rounded-rect.hh"

using namespace creeper;
int main(int argc, char* argv[]) {
    auto application = new QApplication { argc, argv };

    namespace w = pro::widget;
    const auto base = std::tuple { w::MinimumSize { 100, 30 }, w::MaximumSize { 500, 300 } };

    namespace rr = pro::rounded_rect;
    namespace fb = pro::filled_button;
    (new Widget { w::Layout { new Row { pro::linear::Widgets { {
         { new RoundedRect {
             base,
             rr::Background { 0, 200, 0, 255 },
             rr::BorderColor { 200, 0, 0, 255 },
             rr::BorderWidth { 5 },
             rr::Radius { 25 },
         } },
         { new FilledButton {
             base,
             fb::Text { "This Button" },
             fb::Clickable { [] { std::println(std::cout, "Clicked This Button"); } },
         } },
     } } } } })
        ->show();

    return application->exec();
}
