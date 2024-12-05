#include "window.hh"

#include <qcheckbox.h>

using namespace creeper;

struct Window::Impl { };

Window::Window()
    : pimpl_(new Impl)
    , MainWindow() {
    auto horizon = new QHBoxLayout;
    horizon->addWidget(new QCheckBox { "checkbox" });

    auto main = new QWidget;
    main->setLayout(horizon);
    setCentralWidget(main);
}

Window::~Window() { delete pimpl_; }