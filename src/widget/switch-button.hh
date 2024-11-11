#pragma once

#include "setting/theme.hh"
#include "widget/widget.hh"

#include <qabstractbutton.h>
#include <qevent.h>
#include <qpainter.h>
#include <qpropertyanimation.h>

namespace creeper {

// thanks to https://stackoverflow.com/a/38102598
// modify a lot
class SwitchButton : public Extension<QAbstractButton> {
    Q_OBJECT
    Q_PROPERTY(int Progress READ readProgress WRITE writeProgress)
public:
    SwitchButton(QWidget* parent = nullptr)
        : Extension(parent) {
        animation_ = std::make_unique<QPropertyAnimation>(this, "Progress", this);
        animation_->setEasingCurve(QEasingCurve::OutCubic);
        animation_->setDuration(100);
        setFixedSize({ 100, 30 });
    }

    void paintEvent(QPaintEvent* event) override {
        const auto black = Qt::black;

        const auto enabled = QWidget::isEnabled();

        const auto ballRadius = QWidget::height() * 0.45;
        const auto lineRadius = ballRadius * 0.75;

        const auto leftCenter = QPoint(QWidget::height() / 2, QWidget::height() / 2);
        const auto rightCenter = QPoint(QWidget::width() - QWidget::height() / 2, QWidget::height() / 2);
        const auto currentCenter = QPoint(progress_, QWidget::height() / 2);

        auto painter = QPainter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(Qt::NoPen);

        if (!enabled) {
            const auto point0 = leftCenter - QPoint(lineRadius, lineRadius);
            const auto point1 = rightCenter + QPoint(lineRadius, lineRadius);
            painter.setBrush(black);
            painter.setOpacity(0.12);
            painter.drawRoundedRect(QRect(point0, point1), lineRadius, lineRadius);

            const auto point2 = currentCenter - QPoint(ballRadius, ballRadius);
            const auto point3 = currentCenter + QPoint(ballRadius, ballRadius);
            painter.setOpacity(1.0);
            painter.setBrush({ disableGrey_ });
            painter.drawEllipse(QRect(point2, point3));

            return;
        }

        painter.setOpacity(0.75);

        const auto lineLeft = leftCenter - QPoint(lineRadius, lineRadius);
        const auto lineCurrentLeft = currentCenter + QPoint(lineRadius, lineRadius);
        painter.setBrush(QColor(primary200_));
        painter.drawRoundedRect(QRect(lineLeft, lineCurrentLeft), lineRadius, lineRadius);

        const auto lineRight = rightCenter + QPoint(lineRadius, lineRadius);
        const auto lineCurrentRight = currentCenter - QPoint(lineRadius, lineRadius);
        painter.setBrush(QColor(enableGrey_));
        painter.drawRoundedRect(QRect(lineCurrentRight, lineRight), lineRadius, lineRadius);

        painter.setOpacity(1.0);

        const auto ballLeft = currentCenter - QPoint(ballRadius, ballRadius);
        const auto ballRight = currentCenter + QPoint(ballRadius, ballRadius);
        const auto ballColor = switchStatus_ ? primary400_ : enableGrey_;
        painter.setBrush(QColor(ballColor));
        painter.drawEllipse(QRect(ballLeft, ballRight));
    }

    void mouseReleaseEvent(QMouseEvent* event) override {
        if (event->button() & Qt::LeftButton) {
            switchStatus_ = !switchStatus_;

            const auto left = QWidget::height() / 2;
            const auto right = QWidget::width() - QWidget::height() / 2;

            animation_->setStartValue(readProgress());
            animation_->setEndValue(switchStatus_ ? right : left);
            animation_->start();
        }
        QAbstractButton::mouseReleaseEvent(event);
    }

    void setFixedSize(QSize size) {
        progress_ = switchStatus_ ? size.width() - size.height() / 2 : size.height() / 2;
        QWidget::setFixedSize(size);
    }

    void enterEvent(QEnterEvent* event) override {
        QWidget::setCursor(Qt::PointingHandCursor);
        QWidget::enterEvent(event);
    }

    bool switched() const {
        return switchStatus_;
    }

    void reloadTheme() override {
        primary200_ = Theme::color("primary200");
        primary400_ = Theme::color("primary400");
    }

private:
    std::unique_ptr<QPropertyAnimation> animation_;

    bool switchStatus_ = false;
    uint16_t progress_ = 0;

    uint32_t disableGrey_ = 0xbdbdbd;
    uint32_t enableGrey_ = 0xd5d5d5;
    uint32_t primary200_ = 0x7c55bb;
    uint32_t primary400_ = 0x5d34a9;

    int readProgress() const {
        return progress_;
    }

    void writeProgress(int offset) {
        progress_ = offset;
        update();
    }
};

}