#pragma once

#include "creeper-qt/utility/pimpl.hh"
#include "creeper-qt/widget/widget.hh"
#include <qmainwindow.h>

namespace creeper {

class MainWindow : public Extension<QMainWindow> {
    CREEPER_WIDGET_PIMPL_DEFINTION(MainWindow);
    Q_OBJECT
public:
    void moveCenter();
    void reloadTheme() override;
};

}