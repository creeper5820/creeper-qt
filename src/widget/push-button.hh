#pragma once

#include "setting/theme.hh"
#include "widget/widget.hh"

#include <qpushbutton.h>

namespace creeper {

class PushButton : public Extension<QPushButton> {
    Q_OBJECT
public:
    explicit PushButton(QWidget* parent = nullptr)
        : Extension("default", parent) {
        loadStyleFromFile(Theme::qss("push-button"));
    }

private:
};

}