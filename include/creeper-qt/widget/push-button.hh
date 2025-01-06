#pragma once

#include "creeper-qt/utility/pimpl.hh"
#include "creeper-qt/widget/widget.hh"

#include <qpushbutton.h>

#include <limits>

namespace creeper {

class PushButton : public Extension<QPushButton> {
    CREEPER_WIDGET_PIMPL_DEFINTION(PushButton);
    Q_OBJECT

public:
    /// @brief radius = ratio * height
    void setRadiusRatio(double ratio);
    void setBorderWidth(double width);

    void setWaterColor(QColor color);
    void setBorderColor(QColor color);
    void setTextColor(QColor color);
    void setBackgroundColor(QColor color);

    void disableBackground();
    void enableBackground();

    // Water ripple animation
    void enableAnimation();
    void disableAnimation();
    void setDiffusionStep(int step);

    void reloadTheme() override;

protected:
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
};

struct PushButtonStyle {
    std::optional<QString> text;
    std::optional<QSize> size;
    std::optional<QFont> font;
    std::optional<uint32_t> waterColor;
    std::optional<uint32_t> borderColor;
    std::optional<uint32_t> backgroundColor;
    std::optional<uint32_t> textColor;
    std::optional<double> radiusRatio;
    std::optional<double> borderWidth;
    std::optional<bool> autoTheme;
    std::optional<bool> animation;
    std::optional<bool> background;
    void operator()(PushButton& button);
};

}