#pragma once

#include <qpainter.h>
#include <qwidget.h>

namespace creeper {

namespace internal {

    /// @todo 边界的角那里一坨，不知道是不是Qt的锅
    class ShapeWidget : public QWidget {
    public:
        ShapeWidget(QWidget* parent = nullptr)
            : QWidget(parent) { }
        void setBackground(const QColor& color) { background_ = color, update(); }
        void setBorderColor(const QColor& color) { borderColor_ = color, update(); }
        void setBorderWidth(double width) { borderWidth_ = width, update(); }

    protected:
        void setupPainter(QPainter& painter) {
            painter.setPen(QPen { borderColor_, borderWidth_ });
            painter.setBrush(background_);
            painter.setRenderHint(QPainter::Antialiasing);
        }

        QColor background_ = 0xffffffff;
        QColor borderColor_ = 0xffffffff;

        double borderWidth_ = 1;
    };
}

class Rectangle : public internal::ShapeWidget {
public:
    Rectangle(QWidget* parent = nullptr)
        : ShapeWidget(parent) { }

protected:
    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        setupPainter(painter);
        painter.drawRect(rect());
    }
};

class RoundedRectangle : public internal::ShapeWidget {
public:
    RoundedRectangle(QWidget* parent = nullptr)
        : ShapeWidget(parent) { }

    void setRadius(double radius) { radius_ = radius, update(); }

protected:
    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        setupPainter(painter);
        painter.drawRoundedRect(rect(), radius_, radius_);
    }

private:
    double radius_ = 10;
};

}