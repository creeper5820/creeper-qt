#pragma once

#include "creeper-qt/utility/pimpl.hh"
#include "creeper-qt/widget/widget.hh"
#include <qlineedit.h>

namespace creeper {

class LineEdit : public Extension<QLineEdit> {
    Q_OBJECT
    CREEPER_WIDGET_PIMPL_DEFINTION(LineEdit)

public:
    void setIcon(const QIcon& icon);
    void enableIcon();
    void disableIcon();

    void enableBackground();
    void disableBackground();

    void reloadTheme() override;

protected:
    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
};

}