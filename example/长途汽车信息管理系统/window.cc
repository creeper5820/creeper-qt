#include "window.hh"
#include "page/login.hh"
#include "page/workbench/workbench.hh"

#include <creeper-qt/widget/label.hh>
#include <creeper-qt/widget/push-button.hh>
#include <qdebug.h>
#include <qstackedwidget.h>

using namespace creeper;

struct Window::Impl {
    /// @note remember raii
    QStackedWidget stack;

    Login loginPage;
    Workbench workbench;
};

Window::Window()
    : pimpl_(new Impl)
    , MainWindow() {

    pimpl_->stack.addWidget(&pimpl_->loginPage);
    pimpl_->stack.addWidget(&pimpl_->workbench);
    setCentralWidget(&pimpl_->stack);

    connect(&pimpl_->loginPage, &Login::sendLoginRequest,
        [this](const QString& username, const QString& password) {
            pimpl_->stack.setCurrentIndex(1);
        });

    connect(&pimpl_->workbench, &Workbench::logout, [this] { //
        pimpl_->stack.setCurrentIndex(0);
    });
}

Window::~Window() { delete pimpl_; }
