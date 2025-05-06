#include "concave-slider.hh"
#include "creeper-qt/utility/pid.hh"

#include <qevent.h>
#include <qpainter.h>
#include <qpicture.h>
#include <qtimer.h>

using namespace creeper;

struct ConcaveSlider::Impl {
public:
    QPicture makeSliderCanvas(int width, int height) {
        auto picture = QPicture();
        auto painter = QPainter(&picture);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);

        const auto radius = 0.5 * height, lineRadius = 0.2 * radius;
        const auto x0 = radius - lineRadius, x1 = width - radius + lineRadius;
        const auto y0 = radius - lineRadius, y1 = height - radius + lineRadius;
        painter.setBrush({ lineColor });
        painter.drawRoundedRect({ QPointF(x0, y0), QPointF(x1, y1) }, lineRadius, lineRadius);

        const auto ratio = static_cast<double>(value_ - minimum) / (maximum - minimum);
        const auto target = 2 * radius + ratio * (width - 2 * radius);
        if (firstFrame) currentLength = target, firstFrame = false;
        currentLength = updateWithPid(currentLength, target, 0.1);
        painter.setBrush({ foreground });
        painter.drawRoundedRect(QRectF(0, 0, currentLength, height), radius, radius);

        if (std::abs(currentLength - target) < 0.01) timer.stop();
        return picture;
    }

public:
    QTimer timer;

    uint32_t foreground;
    uint32_t lineColor;

    int value_ = 0;
    int minimum = 0;
    int maximum = 100;

    bool pressed = false;
    bool firstFrame = true;

private:
    double currentLength;
};

/// PUBLIC
ConcaveSlider::ConcaveSlider(QWidget* parent)
    : Extension(parent)
    , pimpl_(new Impl()) {
    connect(&pimpl_->timer, &QTimer::timeout, [this] { update(); });
    reloadTheme();
}

ConcaveSlider::~ConcaveSlider() { delete pimpl_; }

void ConcaveSlider::reloadTheme() {
    pimpl_->foreground = Theme::color("primary400");
    pimpl_->lineColor = Theme::color("primary050");
}

int ConcaveSlider::minimum() const { return pimpl_->minimum; }

int ConcaveSlider::maximum() const { return pimpl_->maximum; }

ConcaveSlider& ConcaveSlider::setRange(int minimum, int maximum) {
    pimpl_->minimum = minimum, pimpl_->maximum = maximum;
    emit rangeChanged(minimum, maximum);
    return *this;
}

ConcaveSlider& ConcaveSlider::setValue(int value) {
    if (!pimpl_->timer.isActive()) pimpl_->timer.start(refreshIntervalMs_);
    pimpl_->value_ = value;
    emit valueChanged(value);
    return *this;
}
int ConcaveSlider::value() const { return pimpl_->value_; }

/// PROTECTED
void ConcaveSlider::paintEvent(QPaintEvent* event) {
    auto painter = QPainter(this);
    painter.drawPicture(0, 0, pimpl_->makeSliderCanvas(width(), height()));
}

void ConcaveSlider::mouseMoveEvent(QMouseEvent* event) {
    if (pimpl_->pressed) syncValueFromMouseEvent(*event);
    emit sliderMoved(event->x());
}

void ConcaveSlider::mousePressEvent(QMouseEvent* event) {
    pimpl_->pressed = true;
    syncValueFromMouseEvent(*event);
    emit sliderPressed();
}

void ConcaveSlider::mouseReleaseEvent(QMouseEvent* event) {
    pimpl_->pressed = false;
    syncValueFromMouseEvent(*event);
    emit sliderReleased();
}

/// PRIVATE
double ConcaveSlider::radius() const { return width() > height() ? height() / 2. : width() / 2.; }

void ConcaveSlider::syncValueFromMouseEvent(QMouseEvent& event) {
    const double r = radius(), w = width(), x = event.x();
    const double min = pimpl_->minimum, max = pimpl_->maximum;
    const double ratio = std::clamp((x - r) / (w - r * 2.), 0., 1.);
    const double value = ratio * (max - min) + min;
    if (value != pimpl_->value_) setValue(value);
}
