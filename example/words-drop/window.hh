#pragma once

#include <creeper-qt/utility/pimpl.hh>
#include <creeper-qt/widget/main-window.hh>

class Window : public creeper::MainWindow {
    Q_OBJECT
    CREEPER_PIMPL_DEFINTION(Window);

public:
    void keyPressEvent(QKeyEvent* event) override;
};