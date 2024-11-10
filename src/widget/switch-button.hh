#pragma once

#include "setting/theme.hh"

#include <QtWidgets>

namespace creeper {

class SwitchButton : public QAbstractButton {
    Q_OBJECT
    Q_PROPERTY(int offset READ offset WRITE setOffset)
    Q_PROPERTY(QBrush brush READ brush WRITE setBrush)
public:
    SwitchButton(QWidget* parent = nullptr)
        : QAbstractButton(parent)
        , height_(16)
        , switch_(false)
        , opacity_(0.000)
        , margin_(3)
        , thumb_(0xd5d5d5)
        , animation_(new QPropertyAnimation(this, "offset", this)) {
        setOffset(height_ / 2);
        y_ = height_ / 2;
        setBrush(QColor(0x009688));
    }

    SwitchButton(const QBrush& brush, QWidget* parent = nullptr)
        : QAbstractButton(parent)
        , height_(16)
        , switch_(false)
        , opacity_(0.000)
        , margin_(3)
        , thumb_(0xd5d5d5)
        , animation_(new QPropertyAnimation(this, "offset", this)) {
        setOffset(height_ / 2);
        y_ = height_ / 2;
        setBrush(brush);
    }

    QSize sizeHint() const override {
        return QSize(2 * (height_ + margin_), height_ + 2 * margin_);
    }

    QBrush brush() const {
        return brush_;
    }
    void setBrush(const QBrush& brush) {
        brush_ = brush;
    }

    int offset() const {
        return x_;
    }
    void setOffset(int o) {
        x_ = o;
        update();
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        painter.setPen(Qt::NoPen);
        if (isEnabled()) {
            painter.setBrush(switch_ ? brush() : Qt::black);
            painter.setOpacity(switch_ ? 0.5 : 0.38);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.drawRoundedRect(QRect(margin_, margin_, width() - 2 * margin_, height() - 2 * margin_), 8.0, 8.0);
            painter.setBrush(thumb_);
            painter.setOpacity(1.0);
            painter.drawEllipse(QRectF(offset() - (height_ / 2.), y_ - (height_ / 2.), height(), height()));
        } else {
            painter.setBrush(Qt::black);
            painter.setOpacity(0.12);
            painter.drawRoundedRect(QRect(margin_, margin_, width() - 2 * margin_, height() - 2 * margin_), 8.0, 8.0);
            painter.setOpacity(1.0);
            painter.setBrush(QColor(0xBDBDBD));
            painter.drawEllipse(QRectF(offset() - (height_ / 2.), y_ - (height_ / 2.), height(), height()));
        }
    }

    void mouseReleaseEvent(QMouseEvent* e) override {
        if (e->button() & Qt::LeftButton) {
            switch_ = switch_ ? false : true;
            thumb_ = switch_ ? brush_ : QBrush(0xd5d5d5);
            if (switch_) {
                animation_->setStartValue(height_ / 2);
                animation_->setEndValue(width() - height_);
                animation_->setDuration(120);
                animation_->start();
            } else {
                animation_->setStartValue(offset());
                animation_->setEndValue(height_ / 2);
                animation_->setDuration(120);
                animation_->start();
            }
        }
        QAbstractButton::mouseReleaseEvent(e);
    }

    inline void enterEvent(QEvent* e) {
        setCursor(Qt::PointingHandCursor);
        QAbstractButton::enterEvent(static_cast<QEnterEvent*>(e));
    }

private:
    bool switch_;
    qreal opacity_;

    int x_, y_;
    int height_;
    int margin_;

    QBrush thumb_;
    QBrush brush_;

    QPropertyAnimation* animation_ = nullptr;
};

}
