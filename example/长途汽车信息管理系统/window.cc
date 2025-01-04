#include "window.hh"

using namespace creeper;

struct Window::Impl { };

Window::Window()
    : pimpl_(new Impl)
    , MainWindow() { }

Window::~Window() { delete pimpl_; }
