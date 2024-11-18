#pragma once

#include "../setting/style-template.hh"
#include "../utility/pid.hh"
#include "../widget/widget.hh"

#include <qevent.h>
#include <qicon.h>
#include <qlineedit.h>
#include <qpainter.h>
#include <qtimer.h>

namespace creeper {

class LineEdit : public Extension<QLineEdit> {
    Q_OBJECT
public:
    LineEdit(QWidget* parent = nullptr)
        : Extension(parent) {
        connect(&animationTimer_, &QTimer::timeout, [this] { update(); });
        reloadTheme();
    }

    void reloadTheme() override {
        background_ = Theme::color("primary100");
        border_ = Theme::color("primary200");
        font_ = QFont("monospace", 12, QFont::Normal);

        auto qss = QString(style::LineEdit);
        Extension::setStyleSheet(qss);

        Extension::setFont(font_);
        Extension::setTextMargins(20, 10, 20, 10);
    }

    void enableIcon() {
        drawIcon_ = true;
        setTextMargins_ = false;
    }
    void disableIcon() {
        drawIcon_ = false;
        setTextMargins_ = false;
    }
    void setIcon(const QIcon& icon) {
        icon_ = icon;
        enableIcon();
    }

    void enableBackground() { drawBackground_ = true; }
    void disableBackground() { drawBackground_ = false; }

protected:
    void paintEvent(QPaintEvent* event) override {
        auto painter = QPainter(this);
        if (drawBackground_) backgroundPaintEvent(painter);
        if (drawIcon_) iconPaintEvent(painter);
        if (!setTextMargins_) {
            setTextMargins(drawIcon_ ? height() : 20, 10, 20, 10);
            setTextMargins_ = true;
        }
        Extension::paintEvent(event);
    }

    void enterEvent(QEnterEvent* event) override {
        mouseHover_ = true;
        if (!animationTimer_.isActive()) animationTimer_.start(10);
        Extension::enterEvent(event);
    }

    void leaveEvent(QEvent* event) override {
        mouseHover_ = false;
        if (!animationTimer_.isActive()) animationTimer_.start(10);
        Extension::leaveEvent(event);
    }

private:
    void backgroundPaintEvent(QPainter& painter) {
        const auto width = Extension::width();
        const auto height = Extension::height();

        static double opacity = 0.5;
        const auto target = mouseHover_ ? 0.8 : 0.5;
        opacity = updateWithPid(opacity, target, 0.1);
        if (std::abs(opacity - target) < 0.001) animationTimer_.stop();

        painter.setPen(Qt::NoPen);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setBrush(QColor(background_));
        painter.setOpacity(opacity);

        const auto rect = QRect(0, 0, width, height);
        const auto radius = 0.5 * height;
        painter.drawRoundedRect(rect, radius, radius);
    }

    void iconPaintEvent(QPainter& painter) {
        const auto width = Extension::width();
        const auto height = Extension::height();
        const auto iconPosition = QPoint(height * 0.25, height * 0.25);
        const auto iconPixmap = icon_.pixmap(height * 0.5, height * 0.5);
        painter.setOpacity(1);
        painter.drawPixmap(iconPosition, iconPixmap);
    }

private:
    QIcon icon_;
    QFont font_;

    QTimer animationTimer_ { this };

    uint32_t background_;
    uint32_t border_;

    bool drawBackground_ = true;
    bool drawIcon_ = false;
    bool mouseHover_ = false;
    bool setTextMargins_ = false;
};

}