#pragma once

#include <qpainter.h>
#include <qwidget.h>

namespace creeper {

namespace internal {
    inline void makeShapePainter(QPainter& painter, const QColor& color) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(color);
        painter.setRenderHint(QPainter::Antialiasing);
    }
}

class Rectangle : public QWidget {
public:
    Rectangle(QWidget* parent = nullptr)
        : QWidget(parent) { }
    void setColor(const QColor& color) { background_ = color, update(); }

protected:
    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        internal::makeShapePainter(painter, background_);
        painter.drawRect(rect());
    }

private:
    QColor background_;
};

class RoundedRectangle : public QWidget {
public:
    RoundedRectangle(QWidget* parent = nullptr)
        : QWidget(parent) { }
    void setColor(const QColor& color) { color_ = color, update(); }
    void setRoundedRadius(int radius) { radius_ = radius, update(); }

protected:
    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        internal::makeShapePainter(painter, color_);
        painter.drawRoundedRect(rect(), radius_, radius_);
    }

private:
    QColor color_;
    int radius_ = 10;
};

}