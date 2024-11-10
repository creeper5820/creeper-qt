#pragma once

#include <qapplication.h>
#include <qlayout.h>
#include <qmainwindow.h>
#include <qscreen.h>

#include "setting/theme.hh"
#include "widget/widget.hh"

namespace creeper {

class MainWindow : public Extension<QMainWindow> {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr)
        : Extension(parent) {

        setMinimumWidth(1440);
        setMinimumHeight(720);

        loadStyleFromFile(Theme::qss("main-window"));
    }

    void moveCenter() {
        const auto screenCenter = screen()->geometry().center();
        const auto frameCenter = frameGeometry().center();
        QMainWindow::move(screenCenter - frameCenter);
    }

private:
};

}