// thanks for https://stackoverflow.com/a/38102598
// modify a lot

#include "switch-button.hh"

#include "creeper-qt/setting/theme.hh"
#include "creeper-qt/utility/pid.hh"

#include <qabstractbutton.h>
#include <qevent.h>
#include <qpainter.h>
#include <qpropertyanimation.h>
#include <qtimer.h>

using namespace creeper;

AbstractSwitchButton::AbstractSwitchButton(QWidget* parent)
    : Extension(parent) {
    connect(&animationTimer_, &QTimer::timeout, [this] { update(); });
    reloadTheme();
}

void AbstractSwitchButton::setSwitchStatus(bool switchStatus) {
    switchStatus_ = switchStatus;
    update();
}

bool AbstractSwitchButton::switched() const { return switchStatus_; }

void AbstractSwitchButton::reloadTheme() {
    lightPrimary_ = Theme::color("primary300");
    heavyPrimary_ = Theme::color("primary400");
}

void AbstractSwitchButton::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() & Qt::LeftButton) {
        switchStatus_ = !switchStatus_;
        if (!animationTimer_.isActive()) animationTimer_.start(refreshIntervalMs_);
    }
    QAbstractButton::mouseReleaseEvent(event);
}

void AbstractSwitchButton::enterEvent(QEvent* event) {
    setCursor(Qt::PointingHandCursor);
    Extension::enterEvent(event);
}

ConvexSwitchButton::ConvexSwitchButton(QWidget* parent)
    : AbstractSwitchButton(parent) { }

void ConvexSwitchButton::paintEvent(QPaintEvent* event) {
    const double h = height();
    const double w = width();
    const auto enabled = isEnabled();

    const auto r0 = h * 0.45;
    const auto r1 = r0 * 0.75;

    const auto leftCenter = QPointF(h / 2., h / 2.);
    const auto rightCenter = QPointF(w - h / 2., h / 2.);

    const double target = switchStatus_ ? 1 : 0;
    const auto currentCenter = QPointF(h / 2. + progress_ * (w - h), h / 2.);

    progress_ = updateWithPid(progress_, target, 0.1);
    if (std::abs(progress_ - target) < 0.001) animationTimer_.stop();

    auto painter = QPainter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setOpacity(1.0);

    if (!enabled) {
        const auto p0 = leftCenter - QPointF(r1, r1);
        const auto p1 = rightCenter + QPointF(r1, r1);
        painter.setBrush({ heavyGrey_ + 0x333333 });
        painter.drawRoundedRect(QRectF(p0, p1), r1, r1);

        const auto p2 = currentCenter - QPointF(r0, r0);
        const auto p3 = currentCenter + QPointF(r0, r0);
        painter.setBrush({ heavyGrey_ });
        painter.drawEllipse(QRectF(p2, p3));
        return;
    }

    painter.setOpacity(0.75);

    const auto lineLeft = leftCenter - QPointF(r1, r1);
    const auto lineCurrentLeft = currentCenter + QPointF(r1, r1);
    painter.setBrush({ lightPrimary_ });
    painter.drawRoundedRect(QRectF(lineLeft, lineCurrentLeft), r1, r1);

    const auto lineRight = rightCenter + QPointF(r1, r1);
    const auto lineCurrentRight = currentCenter - QPointF(r1, r1);
    painter.setBrush({ lightGrey_ });
    painter.drawRoundedRect(QRectF(lineCurrentRight, lineRight), r1, r1);

    painter.setOpacity(1.0);

    const auto ballLeft = currentCenter - QPointF(r0, r0);
    const auto ballRight = currentCenter + QPointF(r0, r0);
    const auto ballColor = switchStatus_ ? heavyPrimary_ : lightGrey_;
    painter.setBrush({ ballColor });
    painter.drawEllipse(QRectF(ballLeft, ballRight));
}

ConcaveSwitchButton::ConcaveSwitchButton(QWidget* parent)
    : AbstractSwitchButton(parent) { }

void ConcaveSwitchButton::paintEvent(QPaintEvent* event) {
    const auto enabled = isEnabled();
    const auto h = height();
    const auto w = width();

    const auto r0 = h * 0.45;
    const auto r1 = r0 * 0.75;

    const auto leftCenter = QPointF(h / 2., h / 2.);
    const auto rightCenter = QPointF(w - h / 2., h / 2.);

    const double target = switchStatus_ ? 1 : 0;
    const auto currentCenter = QPointF(h / 2. + progress_ * (w - h), h / 2.);

    progress_ = updateWithPid(progress_, target, 0.1);
    if (std::abs(progress_ - target) < 0.001) animationTimer_.stop();

    auto painter = QPainter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setOpacity(1.0);

    if (!enabled) {
        const auto p0 = leftCenter - QPointF(r0, r0);
        const auto p1 = rightCenter + QPointF(r0, r0);
        painter.setBrush({ heavyGrey_ + 0x333333 });
        painter.drawRoundedRect(QRectF(p0, p1), r1, r1);

        const auto p2 = currentCenter - QPointF(r1, r1);
        const auto p3 = currentCenter + QPointF(r1, r1);
        painter.setBrush({ heavyGrey_ });
        painter.drawEllipse(QRectF(p2, p3));
        return;
    }

    painter.setOpacity(1);

    const auto lineLeft = leftCenter - QPointF(r0, r0);
    const auto lineRight = rightCenter + QPointF(r0, r0);

    painter.setBrush({ switchStatus_ ? heavyPrimary_ : heavyGrey_ });
    painter.drawRoundedRect(QRectF(lineLeft, lineRight), r0, r0);

    const auto border = r0 * 0.2;
    const auto lineInsideLeft = lineLeft + QPointF(border, border);
    const auto lineInsideRight = lineRight - QPointF(border, border);
    const auto r2 = r0 - border;

    if (!switchStatus_) {
        painter.setBrush({ lightGrey_ });
        painter.drawRoundedRect(QRectF(lineInsideLeft, lineInsideRight), r2, r2);
    }

    const auto ballLeft = currentCenter - QPointF(r1, r1);
    const auto ballRight = currentCenter + QPointF(r1, r1);
    const auto ballColor = switchStatus_ ? 0xffffff : heavyGrey_;
    painter.setBrush({ ballColor });
    painter.drawEllipse(QRectF(ballLeft, ballRight));
}
