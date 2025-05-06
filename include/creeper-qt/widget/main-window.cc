#include "main-window.hh"

#include <qapplication.h>
#include <qlayout.h>
#include <qmainwindow.h>
#include <qscreen.h>

#include "creeper-qt/setting/style-template.hh"
#include "creeper-qt/setting/theme.hh"

using namespace creeper;

struct MainWindow::Impl {
    QColor background;
};

MainWindow::MainWindow(QWidget* parent)
    : Extension(parent)
    , pimpl_(new Impl) {
    reloadTheme();
}

MainWindow::~MainWindow() { delete pimpl_; }

void MainWindow::moveCenter() {
    const auto screenCenter = screen()->geometry().center();
    const auto frameCenter = frameGeometry().center();
    QMainWindow::move(screenCenter - frameCenter);
}

void MainWindow::reloadTheme() {
    pimpl_->background = Theme::color("background");
    auto style = style::mainWindow.arg(pimpl_->background.name());
    Extension::setStyleSheet(style);
}
