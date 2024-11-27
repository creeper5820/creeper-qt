#pragma once

#include <qapplication.h>
#include <qlayout.h>
#include <qmainwindow.h>
#include <qscreen.h>

#include "creeper-qt/setting/style-template.hh"
#include "creeper-qt/setting/theme.hh"
#include "creeper-qt/widget/widget.hh"

namespace creeper {

class MainWindow : public Extension<QMainWindow> {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr)
        : Extension(parent) {
        setMinimumWidth(1440);
        setMinimumHeight(720);
        reloadTheme();
    }

    void moveCenter() {
        const auto screenCenter = screen()->geometry().center();
        const auto frameCenter = frameGeometry().center();
        QMainWindow::move(screenCenter - frameCenter);
    }

    void reloadTheme() override {
        background_ = Theme::color("background");
        Extension::setStyleSheet(QString(style::MainWindow).arg(QColor(background_).name()));
    }

private:
    uint32_t background_ { 0xffffff };
};

}