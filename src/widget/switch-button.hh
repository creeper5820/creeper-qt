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
        animation_ = std::make_unique<QPropertyAnimation>(this, "Progress");
        animation_->setEasingCurve(QEasingCurve::OutCubic);
        animation_->setDuration(100);
        setFixedSize({ 100, 30 });
    }

    void paintEvent(QPaintEvent* event) override {
        const auto black = Qt::black;

        const auto enabled = Extension::isEnabled();
        const auto h = Extension::height();
        const auto w = Extension::width();

        const auto r0 = h * 0.45;
        const auto r1 = r0 * 0.75;

        const auto leftCenter = QPoint(h / 2, h / 2);
        const auto rightCenter = QPoint(w - h / 2, h / 2);
        const auto currentCenter = QPoint(progress_, h / 2);

        auto painter = QPainter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(Qt::NoPen);

        if (!enabled) {
            const auto p0 = leftCenter - QPoint(r1, r1);
            const auto p1 = rightCenter + QPoint(r1, r1);
            painter.setBrush(black);
            painter.setOpacity(0.12);
            painter.drawRoundedRect(QRect(p0, p1), r1, r1);

            const auto p2 = currentCenter - QPoint(r0, r0);
            const auto p3 = currentCenter + QPoint(r0, r0);
            painter.setOpacity(1.0);
            painter.setBrush({ disableGrey_ });
            painter.drawEllipse(QRect(p2, p3));

            return;
        }

        painter.setOpacity(0.75);

        const auto lineLeft = leftCenter - QPoint(r1, r1);
        const auto lineCurrentLeft = currentCenter + QPoint(r1, r1);
        painter.setBrush(QColor(primary200_));
        painter.drawRoundedRect(QRect(lineLeft, lineCurrentLeft), r1, r1);

        const auto lineRight = rightCenter + QPoint(r1, r1);
        const auto lineCurrentRight = currentCenter - QPoint(r1, r1);
        painter.setBrush(QColor(enableGrey_));
        painter.drawRoundedRect(QRect(lineCurrentRight, lineRight), r1, r1);

        painter.setOpacity(1.0);

        const auto ballLeft = currentCenter - QPoint(r0, r0);
        const auto ballRight = currentCenter + QPoint(r0, r0);
        const auto ballColor = switchStatus_ ? primary400_ : enableGrey_;
        painter.setBrush(QColor(ballColor));
        painter.drawEllipse(QRect(ballLeft, ballRight));
    }

    void mouseReleaseEvent(QMouseEvent* event) override {
        if (event->button() & Qt::LeftButton) {
            const auto left = Extension::height() / 2;
            const auto right = Extension::width() - Extension::height() / 2;
            switchStatus_ = !switchStatus_;

            animation_->setStartValue(readProgress());
            animation_->setEndValue(switchStatus_ ? right : left);
            animation_->start();
        }
        Extension::mouseReleaseEvent(event);
    }

    void setFixedSize(QSize size) {
        progress_ = switchStatus_ ? size.width() - size.height() / 2 : size.height() / 2;
        Extension::setFixedSize(size);
    }

    void enterEvent(QEnterEvent* event) override {
        Extension::setCursor(Qt::PointingHandCursor);
        Extension::enterEvent(event);
    }

    void setSwitchStatus(bool switchStatus) {
        switchStatus_ = switchStatus;
        Extension::update();
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

    void writeProgress(int progress) {
        progress_ = progress;
        Extension::update();
    }
};

}