#pragma once

#include <qpainter.h>
#include <qwidget.h>

namespace creeper {

class Shape : public QWidget {
public:
    using QWidget::QWidget;

    void set_background(const QColor& color) { background_ = color; }

    void set_border_color(const QColor& color) { border_color_ = color; }
    void set_border_width(double width) { border_width_ = width; }

protected:
    QColor background_   = Qt::gray;
    QColor border_color_ = Qt::black;
    double border_width_ = 0.;
};

}
