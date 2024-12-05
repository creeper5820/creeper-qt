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

ConcaveSlider& ConcaveSlider::setRange(int minimum, int maximum) {
    pimpl_->minimum = minimum, pimpl_->maximum = maximum;
    return *this;
}
int ConcaveSlider::minimum() const { return pimpl_->minimum; }

int ConcaveSlider::maximum() const { return pimpl_->maximum; }

ConcaveSlider& ConcaveSlider::setValue(int value) {
    if (!pimpl_->timer.isActive()) pimpl_->timer.start(refreshIntervalMs_);
    pimpl_->value_ = value;
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
    Extension::mouseMoveEvent(event);
}

void ConcaveSlider::mousePressEvent(QMouseEvent* event) {
    pimpl_->pressed = true;
    syncValueFromMouseEvent(*event);
    Extension::mousePressEvent(event);
}

void ConcaveSlider::mouseReleaseEvent(QMouseEvent* event) {
    pimpl_->pressed = false;
    Extension::mouseReleaseEvent(event);
}

/// PRIVATE
double ConcaveSlider::radius() const { return width() > height() ? height() / 2. : width() / 2.; }

void ConcaveSlider::syncValueFromMouseEvent(QMouseEvent& event) {
    const auto ratio = (event.localPos().x() - radius()) / (width() - radius() * 2.);
    setValue(std::clamp(ratio, 0.0, 1.0) * (pimpl_->maximum - pimpl_->minimum) + pimpl_->minimum);
}