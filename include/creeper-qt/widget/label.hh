#pragma once

#include "../widget/widget.hh"

#include <qlabel.h>

namespace creeper {

class Label : public Extension<QLabel> {
    Q_OBJECT
public:
    explicit Label(QWidget* parent = nullptr)
        : Extension(parent) { }

    void reloadTheme() override { }

private:
};

}