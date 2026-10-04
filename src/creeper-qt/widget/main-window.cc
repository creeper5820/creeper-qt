#include "main-window.hh"

using namespace creeper;

struct MainWindow::Impl { };

MainWindow::MainWindow()
    : pimpl { std::make_unique<Impl>() } { }

MainWindow::~MainWindow() = default;
