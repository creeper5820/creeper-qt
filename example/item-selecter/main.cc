#include "window.hh"
#include <qapplication.h>

auto main(int argc, char** argv) -> int {
    creeper::Theme::setTheme("common-green");
    auto app = QApplication { argc, argv };
    auto window = Window {};
    window.setFixedSize(800, 400);
    window.show();
    return app.exec();
}