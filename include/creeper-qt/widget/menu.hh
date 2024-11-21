#pragma once

#include "creeper-qt/widget/widget.hh"

#include <qmenu.h>

namespace creeper {

class Menu : public Extension<QMenu> {
    Q_OBJECT
public:
    explicit Menu(QWidget* parent = nullptr)
        : Extension(parent) { }

    void reloadTheme() override { }

protected:
};

}