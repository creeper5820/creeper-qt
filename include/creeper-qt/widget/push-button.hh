#pragma once

#include "creeper-qt/utility/pimpl.hh"
#include "creeper-qt/widget/widget.hh"

#include <qpushbutton.h>

namespace creeper {

class PushButton : public Extension<QPushButton> {
    CREEPER_WIDGET_PIMPL_DEFINTION(PushButton);
    CREEPER_WIDGET_OVERRIDE_INVOKE_METHOD(PushButton, Extension<QPushButton>);
    Q_OBJECT

public:
    /// @brief radius = ratio * height
    PushButton& setRadiusRatio(double ratio);
    PushButton& setBorderWidth(double width);

    PushButton& setWaterColor(QColor color);
    PushButton& setBorderColor(QColor color);
    PushButton& setTextColor(QColor color);
    PushButton& setBackgroundColor(QColor color);

    PushButton& disableBackground();
    PushButton& enableBackground();

    // Water ripple animation
    PushButton& enableAnimation();
    PushButton& disableAnimation();
    PushButton& setDiffusionStep(int step);

    // Override invoke method
    PushButton& setText(const QString& text);
    PushButton& setIcon(const QIcon& icon);

    void reloadTheme() override;

protected:
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
};

}