#pragma once

#include "widget/widget.hh"

#include <qpushbutton.h>

namespace creeper {

class Button : public Extension<QPushButton> {
    Q_OBJECT
public:
    explicit Button(QWidget* parent = nullptr)
        : Extension("default", parent) {
        loadStyleFromFile(":qss/common-white/push-button.qss");
    }

private:
};

}