#pragma once

#include "setting/theme.hh"
#include "widget/widget.hh"

#include <qevent.h>
#include <qpainter.h>
#include <qpropertyanimation.h>
#include <qpushbutton.h>

namespace creeper {

class PushButton : public Extension<QPushButton> {
    Q_OBJECT
    Q_PROPERTY(int Distance READ readDistance WRITE writeDistance)
public:
    explicit PushButton(QWidget* parent = nullptr)
        : Extension("default", parent) {
        loadStyleFromFile(Theme::qss("push-button"));
        animation_ = std::make_unique<QPropertyAnimation>(this, "Distance");
    }

    void reloadTheme() override {
        loadStyleFromFile(Theme::qss("push-button"));
    }

    void paintEvent(QPaintEvent* event) override {
        Extension::paintEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent* event) override {
        if (event->button() & Qt::LeftButton) {
            const auto clickPoint = event->pos();
        }
        Extension::mouseReleaseEvent(event);
    }

private:
    std::unique_ptr<QPropertyAnimation> animation_;

    QPoint clickPoint_;
    int distance_;

    int readDistance() const {
        return distance_;
    }

    void writeDistance(int distance) {
        distance_ = distance;
        Extension::update();
    }
};

}