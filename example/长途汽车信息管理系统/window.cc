#include "window.hh"
#include "page/login.hh"

#include <creeper-qt/widget/label.hh>
#include <creeper-qt/widget/push-button.hh>

using namespace creeper;

struct Window::Impl { };

Window::Window()
    : pimpl_(new Impl)
    , MainWindow() {

    auto loginPage = new Login;
    setCentralWidget(loginPage);
}

Window::~Window() { delete pimpl_; }
