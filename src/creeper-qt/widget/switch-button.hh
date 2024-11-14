#pragma once

#include "../setting/theme.hh"
#include "../utility/pid.hh"
#include "../widget/widget.hh"

#include <qabstractbutton.h>
#include <qevent.h>
#include <qpainter.h>
#include <qpropertyanimation.h>
#include <qtimer.h>

namespace creeper {

// thanks to https://stackoverflow.com/a/38102598
// modify a lot
class SwitchButton : public Extension<QAbstractButton> {
    Q_OBJECT
public:
    SwitchButton(QWidget* parent = nullptr)
        : Extension(parent) {
        connect(&animationTimer_, &QTimer::timeout, [this] { update(); });
        reloadTheme();
    }

    void setFixedSize(QSize size) {
        progress_ = switchStatus_ ? size.width() - size.height() / 2 : size.height() / 2;
        Extension::setFixedSize(size);
    }

    void setSwitchStatus(bool switchStatus) {
        switchStatus_ = switchStatus;
        Extension::update();
    }

    void setDisabledButtonColor(uint32_t color) {
        disabledButtonColor_ = color;
        Extension::update();
    }

    bool switched() const {
        return switchStatus_;
    }

    void reloadTheme() override {
        switchedLineColor_ = Theme::color("primary200");
        SwitchedButtonColor_ = Theme::color("primary400");
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        const auto black = Qt::black;

        const auto enabled = Extension::isEnabled();
        const auto h = Extension::height();
        const auto w = Extension::width();

        const auto r0 = h * 0.45;
        const auto r1 = r0 * 0.75;

        const auto leftCenter = QPoint(h / 2, h / 2);
        const auto rightCenter = QPoint(w - h / 2, h / 2);

        const double target = switchStatus_ ? w - h / 2 : h / 2;
        const auto currentCenter = QPoint(progress_, h / 2);
        progress_ = updateWithPid(progress_, target, 0.1);
        if (std::abs(progress_ - target) < 0.1)
            animationTimer_.stop();

        auto painter = QPainter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(Qt::NoPen);
        painter.setOpacity(1.0);

        if (!enabled) {
            const auto p0 = leftCenter - QPoint(r1, r1);
            const auto p1 = rightCenter + QPoint(r1, r1);
            painter.setBrush({ disabledButtonColor_ + 0x333333 });
            painter.drawRoundedRect(QRect(p0, p1), r1, r1);

            const auto p2 = currentCenter - QPoint(r0, r0);
            const auto p3 = currentCenter + QPoint(r0, r0);
            painter.setBrush({ disabledButtonColor_ });
            painter.drawEllipse(QRect(p2, p3));
            return;
        }

        painter.setOpacity(0.75);

        const auto lineLeft = leftCenter - QPoint(r1, r1);
        const auto lineCurrentLeft = currentCenter + QPoint(r1, r1);
        painter.setBrush({ switchedLineColor_ });
        painter.drawRoundedRect(QRect(lineLeft, lineCurrentLeft), r1, r1);

        const auto lineRight = rightCenter + QPoint(r1, r1);
        const auto lineCurrentRight = currentCenter - QPoint(r1, r1);
        painter.setBrush({ notSwitchedLineColor_ });
        painter.drawRoundedRect(QRect(lineCurrentRight, lineRight), r1, r1);

        painter.setOpacity(1.0);

        const auto ballLeft = currentCenter - QPoint(r0, r0);
        const auto ballRight = currentCenter + QPoint(r0, r0);
        const auto ballColor = switchStatus_ ? SwitchedButtonColor_ : notSwitchedLineColor_;
        painter.setBrush({ ballColor });
        painter.drawEllipse(QRect(ballLeft, ballRight));
    }

    void mouseReleaseEvent(QMouseEvent* event) override {
        if (event->button() & Qt::LeftButton) {
            switchStatus_ = !switchStatus_;
            if (!animationTimer_.isActive())
                animationTimer_.start(refreshIntervalMs_);
        }
        Extension::mouseReleaseEvent(event);
    }

    void enterEvent(QEnterEvent* event) override {
        Extension::setCursor(Qt::PointingHandCursor);
        Extension::enterEvent(event);
    }

private:
    double readProgress() const {
        return progress_;
    }

    void writeProgress(int progress) {
        progress_ = progress;
        Extension::update();
    }

private:
    QTimer animationTimer_;

    bool switchStatus_ = false;
    double progress_ = 0;

    uint32_t disabledButtonColor_ = 0xbdbdbd;
    uint32_t notSwitchedLineColor_ = 0xd0d0d0d0;
    uint32_t switchedLineColor_ = 0x7c55bb;
    uint32_t SwitchedButtonColor_ = 0x5d34a9;
};

}