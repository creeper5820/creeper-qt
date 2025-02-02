#include "line-edit.hh"

#include "creeper-qt/setting/style-template.hh"
#include "creeper-qt/utility/pid.hh"
#include "creeper-qt/widget/widget.hh"

#include <qevent.h>
#include <qicon.h>
#include <qlineedit.h>
#include <qpainter.h>
#include <qtimer.h>

namespace creeper {

struct LineEdit::Impl {
    void backgroundPaintEvent(QPainter& painter, int width, int height) {
        if (!drawBackground) return;

        const auto target = mouseHover ? 0.8 : 0.5;
        currentOpacity = updateWithPid(currentOpacity, target, 0.1);
        if (std::abs(currentOpacity - target) < 0.001) animationTimer.stop();

        painter.setPen(Qt::NoPen);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setBrush(QColor(background));
        painter.setOpacity(currentOpacity);

        const auto rect = QRectF(0, 0, width, height);
        const auto radius = 0.5 * height;
        painter.drawRoundedRect(rect, radius, radius);
    }

    void iconPaintEvent(QPainter& painter, int width, int height) {
        if (!drawIcon) return;

        const auto iconPosition = QPointF(height * 0.25, height * 0.25);
        const auto iconPixmap = icon.pixmap(height * 0.5, height * 0.5);
        painter.setOpacity(1);
        painter.drawPixmap(iconPosition, iconPixmap);
    }

    QIcon icon;
    QTimer animationTimer;

    uint32_t background;
    uint32_t border;

    bool drawBackground = true;
    bool drawIcon = false;
    bool mouseHover = false;
    bool setTextMargins = false;

    double currentOpacity = 0.5;
};

LineEdit::LineEdit(QWidget* parent)
    : Extension(parent)
    , pimpl_(new Impl) {
    connect(&pimpl_->animationTimer, &QTimer::timeout, [this] { update(); });
    setStyleSheet(style::lineEdit);
    reloadTheme();
}

LineEdit::~LineEdit() { delete pimpl_; }

void LineEdit::reloadTheme() {
    pimpl_->background = Theme::color("primary100");
    pimpl_->border = Theme::color("primary200");
    pimpl_->setTextMargins = false;
}

LineEdit& LineEdit::enableIcon() {
    pimpl_->drawIcon = true;
    pimpl_->setTextMargins = false;
    return *this;
}
LineEdit& LineEdit::disableIcon() {
    pimpl_->drawIcon = false;
    pimpl_->setTextMargins = false;
    return *this;
}
LineEdit& LineEdit::setIcon(const QIcon& icon) {
    pimpl_->icon = icon;
    enableIcon();
    return *this;
}

LineEdit& LineEdit::enableBackground() {
    pimpl_->drawBackground = true;
    return *this;
}
LineEdit& LineEdit::disableBackground() {
    pimpl_->drawBackground = false;
    return *this;
}

LineEdit& LineEdit::setPlaceholderText(const QString& text) {
    Extension::setPlaceholderText(text);
    return *this;
}

void LineEdit::paintEvent(QPaintEvent* event) {
    auto painter = QPainter(this);
    pimpl_->backgroundPaintEvent(painter, width(), height());
    pimpl_->iconPaintEvent(painter, width(), height());
    if (!pimpl_->setTextMargins) {
        const auto h = height();
        const auto m = pimpl_->drawBackground ? 0.5 * h : 0.1 * h;
        setTextMargins(pimpl_->drawIcon ? h : m, 0.1 * h, m, 0.1 * h);
        pimpl_->setTextMargins = true;
    }
    Extension::paintEvent(event);
}

void LineEdit::enterEvent(QEvent* event) {
    pimpl_->mouseHover = true;
    if (!pimpl_->animationTimer.isActive()) pimpl_->animationTimer.start(10);
    Extension::enterEvent(event);
}

void LineEdit::leaveEvent(QEvent* event) {
    pimpl_->mouseHover = false;
    if (!pimpl_->animationTimer.isActive()) pimpl_->animationTimer.start(10);
    Extension::leaveEvent(event);
}

}