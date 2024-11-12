#pragma once

#include "widget/widget.hh"

#include <qlineedit.h>

namespace creeper {

class LineEdit : public Extension<QLineEdit> {
    Q_OBJECT
public:
    LineEdit(QWidget* parent = nullptr)
        : Extension(parent) {
        Extension::setPlaceholderText("你好世界 HelloWorld");
        Extension::setFont(QFont("monospace", 10, QFont::Normal));
        reloadTheme();
    }

    void reloadTheme() override {
        Extension::loadStyleFromFile(Theme::qss("line-edit"));
    }
};

}