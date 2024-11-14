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
        animationTimer_ = std::make_unique<QTimer>();
        reloadTheme();
        connect(animationTimer_.get(), &QTimer::timeout, [this] {
            Extension::update();
        });
    }

    void reloadTheme() override {
        font_ = QFont("monospace", 12, QFont::Normal);
        background_ = Theme::color("primary100");
        border_ = Theme::color("primary200");

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
        if (drawBackground_)
            backgroundPaintEvent(painter);
        if (drawIcon_)
            iconPaintEvent(painter);
        if (!setTextMargins_) {
            setTextMargins(drawIcon_ ? height() : 20, 10, 20, 10);
            setTextMargins_ = true;
        }
        Extension::paintEvent(event);
    }

    void enterEvent(QEnterEvent* event) override {
        mouseHover_ = true;
        if (!animationTimer_->isActive())
            animationTimer_->start(10);
        Extension::enterEvent(event);
    }

    void leaveEvent(QEvent* event) override {
        mouseHover_ = false;
        if (!animationTimer_->isActive())
            animationTimer_->start(10);
        Extension::leaveEvent(event);
    }

private:
    void backgroundPaintEvent(QPainter& painter) {
        const auto width = Extension::width();
        const auto height = Extension::height();

        static double opacity = 0.5;
        if (mouseHover_) {
            opacity = updateWithPid(opacity, 0.8, 0.1);
            if (std::abs(opacity - 0.8) < 0.001)
                animationTimer_->stop();
        } else {
            opacity = updateWithPid(opacity, 0.5, 0.1);
            if (std::abs(opacity - 0.5) < 0.001)
                animationTimer_->stop();
        }

        painter.setPen(Qt::NoPen);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setBrush(QColor(background_));
        painter.setOpacity(opacity);

        painter.drawRoundedRect(0, 0, width, height, 0.5 * height, 0.5 * height);
    }

    void iconPaintEvent(QPainter& painter) {
        const auto width = Extension::width();
        const auto height = Extension::height();

        auto iconPosition = QPoint(height * 0.25, height * 0.25);
        painter.setOpacity(1);
        painter.drawPixmap(iconPosition, icon_.pixmap(height * 0.5, height * 0.5));
    }

private:
    QIcon icon_;
    QFont font_;

    std::unique_ptr<QTimer> animationTimer_;

    uint32_t background_;
    uint32_t border_;

    bool drawBackground_ = true;
    bool drawIcon_ = false;
    bool mouseHover_ = false;
    bool setTextMargins_ = false;
};

}