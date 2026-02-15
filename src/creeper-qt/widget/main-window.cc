#include "main-window.hh"

using namespace creeper::main_window::internal;

struct MainWindow::Impl { };

auto MainWindow::paintEvent(QPaintEvent* e) -> void { QMainWindow::paintEvent(e); }

MainWindow::MainWindow()
    : pimpl { std::make_unique<Impl>() } { }

MainWindow::~MainWindow() = default;
