#include <qapplication.h>

#include "widget/main-window.hh"
#include "widget/push-button.hh"

int main(int argc, char* argv[]) {
    auto app = std::make_unique<QApplication>(argc, argv);

    auto button = new creeper::Button;
    auto window = new creeper::MainWindow();

    window->layout()->addWidget(button);
    button->setFixedSize({ 160, 80 });
    button->moveCenter();

    window->show();

    return app->exec();
}