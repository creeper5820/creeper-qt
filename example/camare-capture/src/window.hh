#pragma once

#include <creeper-qt/utility/pimpl.hh>
#include <creeper-qt/widget/main-window.hh>

class Workbench : public creeper::MainWindow {
    CREEPER_WIDGET_PIMPL_DEFINTION(Workbench);
    Q_OBJECT
};