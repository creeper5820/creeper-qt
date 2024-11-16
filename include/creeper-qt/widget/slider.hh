#pragma once

#include "../setting/theme.hh"
#include "../utility/pid.hh"
#include "../widget/widget.hh"

#include <qapplication.h>
#include <qevent.h>
#include <qpainter.h>
#include <qtimer.h>
#include <qwidget.h>

namespace creeper {

class RoundContourWidget : public QWidget {
public:
    explicit RoundContourWidget(QWidget* parent = nullptr)
        : QWidget(parent) {
        setAttribute(Qt::WA_TransparentForMouseEvents, true);
        setWindowFlags(Qt::FramelessWindowHint);
        setAttribute(Qt::WA_NoSystemBackground, true);
        setAttribute(Qt::WA_TranslucentBackground, true);

        roundColor_ = Theme::color("primary300");
    }

    void setRoundContourOpacity(double opacity) {
        sliderBlockShadowOpacity_ = opacity;
        update();
    }

    float roundContourOpacity() const {
        return sliderBlockShadowOpacity_;
    }

    float lastOpacity() const {
        return lastOpacity_;
    }

    bool animationFinished() const {
        return animationFinshed_;
    }

    void setRoundRadius(int radius) {
        roundRadius_ = radius;
        update();
    }

    int roundRadius() const {
        return roundRadius_;
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        auto size = rect().size();

        auto painter = QPainter(this);
        painter.setPen(Qt::NoPen);
        painter.setBrush({ roundColor_ });
        painter.setRenderHint(QPainter::Antialiasing, true);
        lastOpacity_ = updateWithPid(lastOpacity_, sliderBlockShadowOpacity_, 0.05);
        if (qAbs(lastOpacity_ - sliderBlockShadowOpacity_) <= 0.005) {
            animationFinshed_ = true;
            painter.setOpacity(sliderBlockShadowOpacity_);
            lastOpacity_ = sliderBlockShadowOpacity_;
        } else {
            animationFinshed_ = false;
            painter.setOpacity(lastOpacity_);
        }
        painter.drawRoundedRect(0, 0, size.width(), size.height(), roundRadius_, roundRadius_);
    }

private:
    bool animationFinshed_ = false;
    double sliderBlockShadowOpacity_ = 0.0;
    double lastOpacity_ = 0.0;

    int roundRadius_ = 20;

    uint32_t roundColor_;
};

class Slider : public Extension<QWidget> {
public:
    explicit Slider(QWidget* parent = nullptr)
        : Extension(parent) {
        static_cast<QApplication*>(QCoreApplication::instance())->installEventFilter(this);
        setMouseTracking(true);

        sliderColor_ = Theme::color("primary100");
        roundColor_ = Theme::color("primary300");

        setMaximumHeight(20);
        setMaximumWidth(200);

        setLength(length());
        setThickness(height());
        setBLockLength(sliderBlockLength_);
        setBLockThickness(sliderBlockThickness_);

        lastpos_ = sliderBlockLength_ / 2;
        pidTarget_ = lastpos_;

        connect(&animationTimer_, &QTimer::timeout, this, [this]() {
            reloadTheme();
            update();
            roundContourWidget_.update();
            roundContourWidget_.raise();
        });
    };

    void reloadTheme() override {
        sliderColor_ = Theme::color("primary100");
        roundColor_ = Theme::color("primary300");
        update();
    }

    void setOrientation(Qt::Orientation orientation) {
        orientation_ = orientation;
        update();
    };

    Qt::Orientation orientation() const {
        return orientation_;
    };

    void setMinimum(int minimum) {
        minimum_ = minimum;
        update();
    };
    int minimum() const {
        return minimum_;
    };

    void setMaximum(int maximum) {
        maximum_ = maximum;
        update();
    };
    int maximum() const {
        return maximum_;
    };

    void setValue(int value) {
        value_ = value;
        pidTarget_ = static_cast<int>(value_ * (sliderLength_ - sliderBlockLength_ / 2 * 2) / (maximum() - minimum()) + sliderBlockLength_ / 2);

        if (!animationTimer_.isActive())
            animationTimer_.start(10);
    };

    int value() const {
        return value_;
    };

    void setLength(int length) {
        if (orientation_ == Qt::Horizontal) {
            setFixedSize(length, height());
            sliderLength_ = length;
        } else
            setFixedSize(width(), length);
        update();
    }
    int length() const {
        return width();
    }

    void setThickness(int thickness) {
        sliderThickness_ = thickness;
        if (sliderBlockThickness_ < thickness)
            sliderBlockThickness_ = thickness;
        if (thickness > height()) {
            setFixedHeight(thickness);
            roundContourWidget_.setFixedSize(roundContourWidget_.width(), thickness + 2 * sliderBlockBorderShadowSize_);
            roundContourWidget_.setRoundRadius(roundContourWidget_.height() / 2 * sliderRoundRatio_);
        }
        update();
    }
    int Thickness() const {
        return sliderThickness_;
    }

    void setBLockLength(int length) {
        if (length > sliderLength_ / 2)
            length = sliderLength_ / 2;
        sliderBlockLength_ = length;
        if (orientation_ == Qt::Horizontal) {
            roundContourWidget_.setFixedSize(length + 2 * sliderBlockBorderShadowSize_, roundContourWidget_.height());
            roundContourWidget_.setRoundRadius(roundContourWidget_.height() / 2 * sliderRoundRatio_);
        }
        update();
    }
    int bLockLength() const {
        return sliderBlockLength_;
    }

    void setBLockThickness(int thickness) {
        if (thickness < sliderThickness_)
            return;
        if (thickness > height()) {
            sliderBlockThickness_ = thickness;
            setFixedHeight(thickness);
            roundContourWidget_.setFixedSize(roundContourWidget_.width(), thickness + 2 * sliderBlockBorderShadowSize_);
            roundContourWidget_.setRoundRadius(roundContourWidget_.height() / 2 * sliderRoundRatio_);
            return;
        }
        sliderBlockThickness_ = thickness;
        roundContourWidget_.setFixedSize(roundContourWidget_.width(), thickness + 2 * sliderBlockBorderShadowSize_);
        roundContourWidget_.setRoundRadius(roundContourWidget_.height() / 2 * sliderRoundRatio_);
        update();
    }
    int bLockThickness() const {
        return sliderBlockThickness_;
    }

    void setBlockBorderShadowSize_(int size) {
        sliderBlockBorderShadowSize_ = size;
        update();
    }

    int blockBorderShadowSize_() const {
        return sliderBlockBorderShadowSize_;
    }

    void setSliderColor(const QColor& color) {
        sliderColor_ = color.toRgb().value();
        update();
    }
    const QColor& sliderColor() const {
        return QColor::fromRgb(sliderColor_);
    }

    void setRoundColor(const QColor& color) {
        roundColor_ = color.toRgb().value();
        update();
    }
    const QColor& roundColor() const {
        return QColor::fromRgb(roundColor_);
    }

    void setBlockOpacity(double opacity) {
        sliderOpacity_ = opacity;
        update();
    }
    double blockOpacity() const {
        return sliderOpacity_;
    }

    void setBlockShadowOpacity(double opacity) {
        sliderBlockShadowOpacity_ = opacity;
        update();
    }
    double blockShadowOpacity() const {
        return sliderBlockShadowOpacity_;
    }

    void setSliderRoundRatio(double ratio) {
        sliderRoundRatio_ = ratio;
        update();
    }
    double sliderRoundRatio() const {
        return sliderRoundRatio_;
    }

    void setDragEverywhere(bool drag) {
        dragEverywhere_ = drag;
    }
    bool dragEverywhere() const {
        return dragEverywhere_;
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        auto painter = QPainter(this);
        sliderPaintEvent(painter);
        animationPaintEvent(painter);
    };

    void animationPaintEvent(QPainter& painter) {
        const auto size = rect().size();

        lastpos_ = updateWithPid(lastpos_, pidTarget_, 0.1);

        if (lastpos_ <= sliderBlockLength_ / 2) {
            lastpos_ = sliderBlockLength_ / 2;
        } else if (lastpos_ >= size.width() - sliderBlockLength_ / 2) {
            lastpos_ = size.width() - sliderBlockLength_ / 2;
        }

        auto valuepos = value_ * (size.width() - sliderBlockLength_ / 2 * 2) / (maximum() - minimum()) + sliderBlockLength_ / 2;

        painter.setOpacity(sliderOpacity_);

        if (qAbs(lastpos_ - valuepos) <= 2.0 && roundContourWidget_.animationFinished()) {
            animationTimer_.stop();
            pidTarget_ = static_cast<int>(value_ * (size.width() - sliderBlockLength_ / 2 * 2) / (maximum() - minimum()) + sliderBlockLength_ / 2);
            lastpos_ = pidTarget_;
            painter.drawRoundedRect(0, (size.height() - sliderThickness_) / 2, value_ * (sliderLength_ - sliderBlockLength_ / 2 * 2) / (maximum() - minimum()) + sliderBlockLength_ / 2 + sliderBlockLength_ / 2, sliderThickness_, sliderThickness_ / 2 * sliderRoundRatio_, sliderThickness_ / 2 * sliderRoundRatio_);

            painter.setBrush({ roundColor_ });
            painter.setOpacity(sliderOpacity_);
            // painter.drawEllipse(lastpos_ - sliderBlockLength_ / 2, 0, 2 * sliderBlockLength_ / 2, 2 * sliderBlockLength_ / 2);

            painter.drawRoundedRect(lastpos_ - sliderBlockLength_ / 2, (size.height() - sliderBlockThickness_) / 2, sliderBlockLength_, sliderBlockThickness_, sliderBlockThickness_ / 2 * sliderRoundRatio_, sliderBlockThickness_ / 2 * sliderRoundRatio_);

            if (mouseHover_ || (!mousePressed_ && !mouseHover_)) {
                QPoint center(lastpos_ - sliderBlockLength_ / 2, (size.height() - roundContourWidget_.height()) / 2);
                center = mapToGlobal(center);
                center -= QPoint(roundContourWidget_.width() / 2 - sliderBlockLength_ / 2, roundContourWidget_.height() / 2 - roundContourWidget_.height() / 2);

                roundContourWidget_.move(center);
            }

        } else {
            painter.drawRoundedRect(0, (size.height() - sliderThickness_) / 2, lastpos_ + sliderBlockLength_ / 2, sliderThickness_, sliderThickness_ / 2 * sliderRoundRatio_, sliderThickness_ / 2 * sliderRoundRatio_);

            painter.setBrush({ roundColor_ });
            painter.setOpacity(sliderOpacity_);
            // painter.drawEllipse(lastpos_ - sliderBlockLength_/2, 0, 2 * sliderBlockLength_/2, 2 * sliderBlockLength_/2);
            painter.drawRoundedRect(lastpos_ - sliderBlockLength_ / 2, (size.height() - sliderBlockThickness_) / 2, sliderBlockLength_, sliderBlockThickness_, sliderBlockThickness_ / 2 * sliderRoundRatio_, sliderBlockThickness_ / 2 * sliderRoundRatio_);

            if (mouseHover_ || (!mousePressed_ && !mouseHover_)) {
                QPoint center(lastpos_ - sliderBlockLength_ / 2, (size.height() - roundContourWidget_.height()) / 2);
                center = mapToGlobal(center);
                center -= QPoint(roundContourWidget_.width() / 2 - sliderBlockLength_ / 2, roundContourWidget_.height() / 2 - roundContourWidget_.height() / 2);
                roundContourWidget_.move(center);
            }
        }
    }

    void sliderPaintEvent(QPainter& painter) {
        const auto size = rect().size();

        painter.setPen(Qt::NoPen);
        painter.setBrush({ sliderColor_ });
        painter.setOpacity(sliderBackgroundOpacity_);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.drawRoundedRect(0, (size.height() - sliderThickness_) / 2, sliderLength_, sliderThickness_, sliderThickness_ / 2 * sliderRoundRatio_, sliderThickness_ / 2 * sliderRoundRatio_);
    }

    void mousePressEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton && !mouseHover_ && mouseEntered_) {
            mouseHover_ = true;
            value_ = (static_cast<float>(event->pos().x() - sliderBlockLength_ / 2) / static_cast<float>(rect().size().width() - sliderBlockLength_ / 2 * 2) * (maximum() - minimum()));
            if (value_ < minimum_)
                value_ = minimum_;
            if (value_ > maximum_)
                value_ = maximum_;
            pidTarget_ = event->pos().x();
            if (!animationTimer_.isActive()) {
                animationTimer_.start(refreshIntervalMs_);
            }
            if (dragEverywhere_) {
                mousePressed_ = true;
                lastmousePos_ = event->pos().x();
            }
        } else if (event->button() == Qt::LeftButton && mouseHover_ && mouseEntered_) {
            value_ = (static_cast<float>(event->pos().x() - sliderBlockLength_ / 2) / static_cast<float>(rect().size().width() - sliderBlockLength_ / 2 * 2) * (maximum() - minimum()));
            if (value_ < minimum_)
                value_ = minimum_;
            if (value_ > maximum_)
                value_ = maximum_;
            pidTarget_ = event->pos().x();
            mousePressed_ = true;
        }
        Extension::mousePressEvent(event);
    };

    void mouseMoveEvent(QMouseEvent* event) override {
        if ((event->pos().x() - lastpos_) * (event->pos().x() - lastpos_) + (event->pos().y() - size().height() / 2) * (event->pos().y() - size().height() / 2) <= (sliderBlockLength_ / 2 + 2) * (sliderBlockLength_ / 2 + 2)) {
            mouseHover_ = true;
            roundContourWidget_.setRoundContourOpacity(sliderBlockShadowOpacity_);
            if (mousePressed_) {
                value_ = (static_cast<float>(event->pos().x() - sliderBlockLength_ / 2) / static_cast<float>(rect().size().width() - sliderBlockLength_ / 2 * 2) * (maximum() - minimum()));
                if (value_ < minimum_)
                    value_ = minimum_;
                if (value_ > maximum_)
                    value_ = maximum_;
                pidTarget_ = event->pos().x();
            }

            if (!animationTimer_.isActive()) {
                animationTimer_.start(refreshIntervalMs_);
            }
        } else {
            mouseHover_ = false;
            roundContourWidget_.setRoundContourOpacity(0.0);
            if (mousePressed_) {
                value_ = (static_cast<float>(event->pos().x() - lastmousePos_) / static_cast<float>(rect().size().width() - sliderBlockLength_ / 2 * 2) * (maximum() - minimum()));
                if (value_ < minimum_)
                    value_ = minimum_;
                if (value_ > maximum_)
                    value_ = maximum_;
                pidTarget_ = event->pos().x() - lastmousePos_;
            }
            if (!animationTimer_.isActive()) {
                animationTimer_.start(refreshIntervalMs_);
            }
        }
        if (!animationTimer_.isActive() && mousePressed_) {
            value_ = (static_cast<float>(event->pos().x() - sliderBlockLength_ / 2) / static_cast<float>(rect().size().width() - sliderBlockLength_ / 2 * 2) * (maximum() - minimum()));
            if (value_ < minimum_)
                value_ = minimum_;
            if (value_ > maximum_)
                value_ = maximum_;
            pidTarget_ = event->pos().x();
            animationTimer_.start(refreshIntervalMs_);
        }
        Extension::mouseMoveEvent(event);
    };

    bool eventFilter(QObject* obj, QEvent* event) override {
        if (mousePressed_) {
            if (event->type() == QEvent::MouseMove) {
                mouseHover_ = true;
                roundContourWidget_.setRoundContourOpacity(sliderBlockShadowOpacity_);
                auto tempMousePos = static_cast<QMouseEvent*>(event)->pos().x();
                value_ = (static_cast<float>(tempMousePos - sliderBlockLength_ / 2) / static_cast<float>(rect().size().width() - sliderBlockLength_ / 2 * 2) * (maximum() - minimum()));
                if (value_ < minimum_)
                    value_ = minimum_;
                if (value_ > maximum_)
                    value_ = maximum_;
                pidTarget_ = tempMousePos;
                if (!animationTimer_.isActive())
                    animationTimer_.start(refreshIntervalMs_);
            }
            if (event->type() == QEvent::MouseButtonRelease) {
                mousePressed_ = false;
                mouseHover_ = false;
                roundContourWidget_.setRoundContourOpacity(0.0);
                if (!animationTimer_.isActive())
                    animationTimer_.start(refreshIntervalMs_);
            }
        }
        return QObject::eventFilter(obj, event);
    }

    void enterEvent(QEnterEvent* event) override {
        mouseEntered_ = true;
        Extension::enterEvent(event);
    };
    void leaveEvent(QEvent* event) override {
        mouseEntered_ = false;
        mouseHover_ = false;
        roundContourWidget_.setRoundContourOpacity(0.0);
        if (!animationTimer_.isActive()) {
            animationTimer_.start(refreshIntervalMs_);
        }
        Extension::leaveEvent(event);
    };

    void showEvent(QShowEvent* event) override {
        roundContourWidget_.show();
        Extension::showEvent(event);
    };

    void hideEvent(QHideEvent* event) override {
        roundContourWidget_.hide();
        Extension::hideEvent(event);
    };

    void closeEvent(QCloseEvent* event) override {
        roundContourWidget_.close();
        Extension::closeEvent(event);
    };

private:
    bool mouseHover_ = false;
    bool mousePressed_ = false;
    bool mouseEntered_ = false;
    bool dragEverywhere_ = true;

    QTimer animationTimer_;

    uint32_t sliderColor_;
    uint32_t roundColor_;

    double sliderRoundRatio_ = 0.8;

    int sliderLength_ = 200;
    int sliderThickness_ = 20;
    int sliderBlockLength_ = 40;
    int sliderBlockThickness_ = 20;
    int sliderBlockBorderShadowSize_ = 5;

    float pidTarget_ = 0;
    float lastpos_;
    int lastmousePos_;

    double sliderBackgroundOpacity_ = 0.2;
    double sliderOpacity_ = 1.0;
    double sliderBlockShadowOpacity_ = 0.5;

    Qt::Orientation orientation_ = Qt::Horizontal;
    int minimum_ = 0;
    int maximum_ = 100;
    int value_ = 0;

    RoundContourWidget roundContourWidget_;
};
}
