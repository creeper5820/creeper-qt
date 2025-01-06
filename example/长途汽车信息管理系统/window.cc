#include "window.hh"
#include "page/login.hh"
#include "page/workbench/workbench.hh"

#include <creeper-qt/widget/label.hh>
#include <creeper-qt/widget/push-button.hh>
#include <qdebug.h>

using namespace creeper;

struct Window::Impl { };

Window::Window()
    : pimpl_(new Impl)
    , MainWindow() {

    auto loginPage = new Login;
    auto workbench = new Workbench;

    setCentralWidget(loginPage);

    connect(loginPage, &Login::sendLoginRequest,
        [this, workbench, loginPage](const QString& username, const QString& password) {
            qDebug() << "sendLoginRequest, username: " << username << ", password: " << password;
            this->setCentralWidget(workbench);
        });
}

Window::~Window() { delete pimpl_; }
