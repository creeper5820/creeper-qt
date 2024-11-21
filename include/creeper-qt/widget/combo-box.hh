#pragma once

#include "creeper-qt/widget/widget.hh"

#include <qcombobox.h>

namespace creeper {

class ComboBox : public Extension<QComboBox> {
    Q_OBJECT
public:
    explicit ComboBox(QWidget* parent = nullptr)
        : Extension(parent) { }

    void reloadTheme() override { }

protected:
private:
};

}