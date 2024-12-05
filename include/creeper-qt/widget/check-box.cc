#include "creeper-qt/widget/check-box.hh"
#include <qpainter.h>

using namespace creeper;

static inline constexpr uint32_t gray = 0x808080;

struct CheckBox::Impl {
    bool checked;
    uint32_t color;
};

CheckBox::CheckBox(QWidget* parent)
    : pimpl_(new Impl)
    , Extension(parent) { }

CheckBox::~CheckBox() { delete pimpl_; }

void CheckBox::paintEvent(QPaintEvent* event) {
    const auto width = Extension::width();
    const auto height = Extension::height();
    const auto length = std::min(width, height);
    const auto center = QPointF(width / 2., height / 2.);
    const auto vertex0 = center - QPointF(length / 2., length / 2.);
    const auto vertex1 = center + QPointF(length / 2., length / 2.);
    const auto centerReact = QRectF { vertex0, vertex1 };

    auto painter = QPainter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (pimpl_->checked) {
        painter.setPen(Qt::NoPen);
        painter.setBrush({ pimpl_->color });
        painter.drawRoundedRect(centerReact, 0.2 * length, 0.2 * length);
    }
}

void CheckBox::mouseReleaseEvent(QMouseEvent* event) {
    pimpl_->checked = !pimpl_->checked;
    Extension::mouseReleaseEvent(event);
}
