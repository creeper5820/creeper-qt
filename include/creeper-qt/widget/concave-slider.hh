#pragma once

#include "creeper-qt/utility/pimpl.hh"
#include "creeper-qt/widget/widget.hh"

namespace creeper {

class ConcaveSlider : public Extension<QWidget> {
    CREEPER_WIDGET_PIMPL_DEFINTION(ConcaveSlider);
    Q_OBJECT

public:
    void reloadTheme() override;

    ConcaveSlider& setRange(int minimum, int maximum);
    int minimum() const;
    int maximum() const;

    ConcaveSlider& setValue(int value);
    int value() const;

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