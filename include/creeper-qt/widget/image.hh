#pragma once

#include "../widget/widget.hh"

#include <qabstractbutton.h>

namespace creeper {

class Image : public Extension<QAbstractButton> {
public:
    explicit Image(QWidget* parent = nullptr)
        : Extension(parent) {
    }

    void reloadTheme() override {
    }

private:
    QImage image_;
};

}