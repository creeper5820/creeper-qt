#include "window.hh"

auto main(int argc, char* argv[]) -> int {
    creeper::Theme::setTheme("common-green");
    auto app = QApplication(argc, argv);
    auto window = Workbench();
    window.show();
    return app.exec();
}
