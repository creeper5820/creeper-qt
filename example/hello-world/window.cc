#include "window.hh"
#include <creeper-qt/widget/label.hh>

using namespace creeper;

Window::Window()
    : MainWindow() {
    auto label = new Label { "Hello, World!" };
    label->setAlignment(Qt::AlignCenter);
    setCentralWidget(label);
}
