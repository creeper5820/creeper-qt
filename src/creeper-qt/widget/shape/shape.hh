#pragma once

#include <qpainter.h>
#include <qwidget.h>

namespace creeper {

class Shape : public QWidget {
public:
    using QWidget::QWidget;

    void setBackground(const QColor& color) {
        background_ = color;
        update();
    }

    void setBorderColor(const QColor& color) {
        border_color_ = color;
        update();
    }
    void setBorderWidth(double width) {
        border_width_ = width;
        update();
    }

protected:
    QColor background_   = Qt::gray;
    QColor border_color_ = Qt::black;
    double border_width_ = 0.;
};

}
