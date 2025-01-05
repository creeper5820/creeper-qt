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

    void setRadius(int radius) {
        radius_ = radius;
        setFixedSize({ radius_ * 2, radius_ * 2 });
    }

    void setIcon(const QIcon& icon) { icon_ = icon; }

    // default equals radius
    void setIconRatio(float ratio) { iconRatio_ = ratio; }

protected:
    void paintEvent(QPaintEvent* event) override {
        PushButton::paintEvent(event);

        auto painter = QPainter(this);
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::NoBrush);

        const auto pixmap = icon_.pixmap(radius_ * iconRatio_, radius_ * iconRatio_);
        const auto point = QPoint((width() - pixmap.width()) / 2, (height() - pixmap.height()) / 2);
        painter.drawPixmap(point, pixmap);
    }

private:
    QIcon icon_;
    int radius_;
    double iconRatio_ = 1;
};

};