#pragma once

#include "creeper-qt/utility/pimpl.hh"
#include "creeper-qt/widget/widget.hh"
#include <qabstractbutton.h>

namespace creeper {

class CheckBox : public Extension<QAbstractButton> {
    CREEPER_WIDGET_PIMPL_DEFINTION(CheckBox);
    Q_OBJECT
public:
    bool checked() const;
    void setChecked(bool checked);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
};

}