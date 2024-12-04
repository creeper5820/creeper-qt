#pragma once

#include "creeper-qt/utility/pimpl.hh"
#include "creeper-qt/widget/widget.hh"

#include <qpushbutton.h>

namespace creeper {

class PushButton : public Extension<QPushButton> {
    CREEPER_WIDGET_PIMPL_DEFINTION(PushButton);
    Q_OBJECT

public:
    void reloadTheme() override;
    void setWaterColor(uint32_t color);
    void setRadiusRatio(float ratio);

    void disableBackground();
    void enableBackground();

    // Water ripple animation
    void enableAnimation();
    void disableAnimation();
    void setDiffusionStep(int step);

protected:
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
};

}