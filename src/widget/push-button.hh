#pragma once

#include "setting/theme.hh"
#include "widget/widget.hh"

#include <qpushbutton.h>

namespace creeper {

class Button : public Extension<QPushButton> {
    Q_OBJECT
public:
    explicit Button(QWidget* parent = nullptr)
        : Extension("default", parent) {
        auto& theme = Theme::instance();
        loadStyleFromFile(theme.qss("push-button"));
    }

private:
};

}