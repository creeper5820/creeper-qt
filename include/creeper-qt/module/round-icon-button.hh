#pragma once

#include "creeper-qt/widget/push-button.hh"

#include <qpainter.h>

namespace creeper {

class RoundIconButton : public PushButton {
    Q_OBJECT
public:
    explicit RoundIconButton(QWidget* parent = nullptr)
        : PushButton(parent) {
        setRadiusRatio(0.5);
    }

    RoundIconButton& setRadius(int radius) {
        radius_ = radius;
        setFixedSize({ radius_ * 2, radius_ * 2 });
        return *this;
    }
    // default equals radius
    RoundIconButton& setIconRatio(float ratio) {
        ratio_ = ratio;
        return *this;
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        PushButton::paintEvent(event);

        auto painter = QPainter(this);
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::NoBrush);

        const auto icon = PushButton::icon();
        const auto pixmap = icon.pixmap(radius_ * ratio_, radius_ * ratio_);
        const auto point = QPoint((width() - pixmap.width()) / 2, (height() - pixmap.height()) / 2);
        painter.drawPixmap(point, pixmap);
    }

private:
    int radius_;
    double ratio_ = 1;
};

};