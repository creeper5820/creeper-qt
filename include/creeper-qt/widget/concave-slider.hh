#pragma once

#include "creeper-qt/utility/pimpl.hh"
#include "creeper-qt/widget/widget.hh"

namespace creeper {

class ConcaveSlider : public Extension<QWidget> {
    CREEPER_WIDGET_PIMPL_DEFINTION(ConcaveSlider);
    Q_OBJECT
public:
    void reloadTheme() override;

    int minimum() const;
    int maximum() const;
    int value() const;

public slots:
    void setRange(int minimum, int maximum);
    void setValue(int value);

signals:
    void valueChanged(int value);
    void rangeChanged(int min, int max);
    void sliderPressed();
    void sliderReleased();
    void sliderMoved(int position);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    double radius() const;
    void syncValueFromMouseEvent(QMouseEvent& event);
};

}