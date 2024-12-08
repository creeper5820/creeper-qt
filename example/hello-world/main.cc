#include "window.hh"
#include <qapplication.h>

auto main(int argc, char** argv) -> int {
    using namespace creeper;
    Theme::setTheme(Theme::common::green);
    auto app = QApplication { argc, argv };
    auto window = Window {};
    window.show();
    return app.exec();
}