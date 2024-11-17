#pragma once

#include "../setting/theme.hh"
#include "../utility/pid.hh"
#include "../widget/widget.hh"

#include <algorithm>

#include <qapplication.h>
#include <qevent.h>
#include <qnamespace.h>
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

    int roundRadius_ = 0;

    uint32_t roundColor_;
};

class Slider : public Extension<QWidget> {
public:
    explicit Slider(QWidget* parent = nullptr)
        : Extension(parent) {
        static_cast<QApplication*>(QCoreApplication::instance())->installEventFilter(this);
        reloadTheme();
        setMouseTracking(true);

        sliderColor_ = Theme::color("primary100");
        roundColor_ = Theme::color("primary300");

        setLength(width());
        setBLockThickness(height());

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
        if (orientation_ == Qt::Horizontal && orientation == Qt::Vertical) {
            orientation_ = orientation;
            setFixedSize(sliderBlockThickness_, sliderLength_);
        } else if (orientation_ == Qt::Vertical && orientation == Qt::Horizontal) {
            orientation_ = orientation;
            setFixedSize(sliderLength_, sliderBlockThickness_);
        }
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
        pidTarget_ = static_cast<int>(value_ * (sliderLength_ - sliderBlockLength_) / (maximum_ - minimum_) + sliderBlockLength_ / 2);

        if (!animationTimer_.isActive())
            animationTimer_.start(10);
    };

    int value() const {
        return value_;
    };

    void setFixedSize(int width, int height) {
        if (orientation_ == Qt::Horizontal) {
            Extension::setFixedSize(width, height);
            sliderLength_ = width;
            sliderBlockThickness_ = height;
            roundContourWidget_.setFixedSize(sliderBlockLength_ + 2 * sliderBlockBorderShadowSize_, sliderBlockThickness_ + 2 * sliderBlockBorderShadowSize_);
        } else {
            Extension::setFixedSize(width, height);
            sliderLength_ = height;
            sliderBlockThickness_ = width;
            roundContourWidget_.setFixedSize(sliderBlockThickness_ + 2 * sliderBlockBorderShadowSize_, sliderBlockLength_ + 2 * sliderBlockBorderShadowSize_);
        }
        roundContourWidget_.setRoundRadius(sliderBlockThickness_ / 2 + sliderBlockBorderShadowSize_);
        update();
    }

    void setFixedHeight(int height) {
        setFixedSize(width(), height);
    }

    void setFixedWidth(int width) {
        setFixedSize(width, height());
    }

    void setLength(int length) {
        if (length < sliderBlockLength_)
            sliderBlockLength_ = length;
        if (orientation_ == Qt::Horizontal) {
            setFixedSize(length, sliderThickness_);
        } else {
            setFixedSize(sliderThickness_, length);
        }
    }
    int length() const {
        return sliderLength_;
    }

    void setThickness(int thickness) {
        sliderThickness_ = thickness;
        if (sliderBlockThickness_ < sliderThickness_) {
            if (orientation_ == Qt::Horizontal)
                setFixedSize(sliderLength_, sliderThickness_);
            else
                setFixedSize(sliderThickness_, sliderLength_);
        }
    }
    int Thickness() const {
        return sliderThickness_;
    }

    void setBLockLength(int length) {
        sliderBlockLength_ = length;
        if (orientation_ == Qt::Horizontal)
            roundContourWidget_.setFixedSize(sliderBlockLength_ + 2 * sliderBlockBorderShadowSize_, sliderBlockThickness_ + 2 * sliderBlockBorderShadowSize_);
        else
            roundContourWidget_.setFixedSize(sliderBlockThickness_ + 2 * sliderBlockBorderShadowSize_, sliderBlockLength_ + 2 * sliderBlockBorderShadowSize_);
        roundContourWidget_.setRoundRadius(sliderBlockThickness_ / 2 + sliderBlockBorderShadowSize_);

        if (sliderBlockLength_ > sliderLength_) {
            if (orientation_ == Qt::Horizontal)
                setFixedSize(sliderBlockLength_, sliderBlockThickness_);
            else
                setFixedSize(sliderBlockThickness_, sliderBlockLength_);
        }
    }
    int bLockLength() const {
        return sliderBlockLength_;
    }

    void setBLockThickness(int thickness) {
        if (thickness < sliderThickness_)
            sliderBlockThickness_ = sliderThickness_;
        if (orientation_ == Qt::Horizontal) {
            setFixedSize(sliderLength_, thickness);
        } else {
            setFixedSize(thickness, sliderLength_);
        }
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
        roundContourWidget_.setRoundRadius((sliderBlockThickness_ / 2 + sliderBlockBorderShadowSize_) * sliderRoundRatio_);
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
        lastpos_ = updateWithPid(lastpos_, pidTarget_, 0.1);
        auto halfSliderBlockLength = sliderBlockLength_ / 2;

        if (lastpos_ <= halfSliderBlockLength) {
            lastpos_ = halfSliderBlockLength;
        } else if (lastpos_ >= sliderLength_ - halfSliderBlockLength) {
            lastpos_ = sliderLength_ - halfSliderBlockLength;
        }

        painter.setOpacity(sliderOpacity_);

        if (orientation_ == Qt::Horizontal) // 水平布局
            if (qAbs(lastpos_ - (value_ * (sliderLength_ - sliderBlockLength_) / (maximum_ - minimum_) + halfSliderBlockLength)) <= 2.0 && roundContourWidget_.animationFinished()) {
                animationTimer_.stop();
                pidTarget_ = static_cast<int>(value_ * (sliderLength_ - sliderBlockLength_) / (maximum_ - minimum_) + halfSliderBlockLength);
                lastpos_ = pidTarget_;
                painter.drawRoundedRect(0, (sliderBlockThickness_ - sliderThickness_) / 2, value_ * (sliderLength_ - sliderBlockLength_) / (maximum_ - minimum_) + halfSliderBlockLength + halfSliderBlockLength, sliderThickness_, sliderThickness_ / 2 * sliderRoundRatio_, sliderThickness_ / 2 * sliderRoundRatio_);

                painter.setBrush({ roundColor_ });
                painter.setOpacity(sliderOpacity_);

                painter.drawRoundedRect(lastpos_ - halfSliderBlockLength, 0, sliderBlockLength_, sliderBlockThickness_, sliderBlockThickness_ / 2 * sliderRoundRatio_, sliderBlockThickness_ / 2 * sliderRoundRatio_);

                if (mouseHover_ || (!mousePressed_ && !mouseHover_)) {
                    QPoint center(lastpos_ - halfSliderBlockLength, (sliderBlockThickness_ - roundContourWidget_.height()) / 2);
                    center = mapToGlobal(center);
                    center -= QPoint(sliderBlockBorderShadowSize_, 0);

                    roundContourWidget_.move(center);
                }
            } else {
                painter.drawRoundedRect(0, (sliderBlockThickness_ - sliderThickness_) / 2, lastpos_ + halfSliderBlockLength, sliderThickness_, sliderThickness_ / 2 * sliderRoundRatio_, sliderThickness_ / 2 * sliderRoundRatio_);

                painter.setBrush({ roundColor_ });
                painter.setOpacity(sliderOpacity_);
                painter.drawRoundedRect(lastpos_ - halfSliderBlockLength, 0, sliderBlockLength_, sliderBlockThickness_, sliderBlockThickness_ / 2 * sliderRoundRatio_, sliderBlockThickness_ / 2 * sliderRoundRatio_);

                if (mouseHover_ || (!mousePressed_ && !mouseHover_)) {
                    QPoint center(lastpos_ - halfSliderBlockLength, (sliderBlockThickness_ - roundContourWidget_.height()) / 2);
                    center = mapToGlobal(center);
                    center -= QPoint(sliderBlockBorderShadowSize_, 0);
                    roundContourWidget_.move(center);
                }
            }
        else { // 竖直布局
            if (qAbs(lastpos_ - (value_ * (sliderLength_ - sliderBlockLength_) / (maximum_ - minimum_) + halfSliderBlockLength)) <= 2.0 && roundContourWidget_.animationFinished()) {
                animationTimer_.stop();
                pidTarget_ = static_cast<int>(value_ * (sliderLength_ - sliderBlockLength_) / (maximum_ - minimum_) + halfSliderBlockLength);
                lastpos_ = pidTarget_;
                painter.drawRoundedRect((sliderBlockThickness_ - sliderThickness_) / 2, 0, sliderThickness_, value_ * (sliderLength_ - sliderBlockLength_) / (maximum_ - minimum_) + halfSliderBlockLength + halfSliderBlockLength, sliderThickness_ / 2 * sliderRoundRatio_, sliderThickness_ / 2 * sliderRoundRatio_);

                painter.setBrush({ roundColor_ });
                painter.setOpacity(sliderOpacity_);

                painter.drawRoundedRect(0, lastpos_ - halfSliderBlockLength, sliderBlockThickness_, sliderBlockLength_, sliderBlockThickness_ / 2 * sliderRoundRatio_, sliderBlockThickness_ / 2 * sliderRoundRatio_);

                if (mouseHover_ || (!mousePressed_ && !mouseHover_)) {
                    QPoint center(0, lastpos_ - halfSliderBlockLength);
                    center = mapToGlobal(center);
                    center -= QPoint(sliderBlockBorderShadowSize_, sliderBlockBorderShadowSize_);

                    roundContourWidget_.move(center);
                }
            } else {
                painter.drawRoundedRect((sliderBlockThickness_ - sliderThickness_) / 2, 0, sliderThickness_, lastpos_ + halfSliderBlockLength, sliderThickness_ / 2 * sliderRoundRatio_, sliderThickness_ / 2 * sliderRoundRatio_);

                painter.setBrush({ roundColor_ });
                painter.setOpacity(sliderOpacity_);
                painter.drawRoundedRect(0, lastpos_ - halfSliderBlockLength, sliderBlockThickness_, sliderBlockLength_, sliderBlockThickness_ / 2 * sliderRoundRatio_, sliderBlockThickness_ / 2 * sliderRoundRatio_);

                if (mouseHover_ || (!mousePressed_ && !mouseHover_)) {
                    QPoint center(0, lastpos_ - halfSliderBlockLength);
                    center = mapToGlobal(center);
                    center -= QPoint(sliderBlockBorderShadowSize_, sliderBlockBorderShadowSize_);

                    roundContourWidget_.move(center);
                }
            }
        }
    }

    void sliderPaintEvent(QPainter& painter) {
        const auto size = rect().size();

        painter.setPen(Qt::NoPen);
        painter.setBrush({ sliderColor_ });
        painter.setOpacity(sliderBackgroundOpacity_);
        painter.setRenderHint(QPainter::Antialiasing, true);
        if (orientation_ == Qt::Horizontal)
            painter.drawRoundedRect(0, (size.height() - sliderThickness_) / 2, sliderLength_, sliderThickness_, sliderThickness_ / 2 * sliderRoundRatio_, sliderThickness_ / 2 * sliderRoundRatio_);
        else
            painter.drawRoundedRect((size.width() - sliderThickness_) / 2, 0, sliderThickness_, sliderLength_, sliderThickness_ / 2 * sliderRoundRatio_, sliderThickness_ / 2 * sliderRoundRatio_);
    }

    void mousePressEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton && !mouseHover_ && mouseEntered_) {
            mouseHover_ = true;
            mousePressed_ = true;

            if (orientation_ == Qt::Horizontal) {
                pidTarget_ = event->pos().x();
                if (dragEverywhere_) {
                    lastmousePos_ = event->pos().x();
                }
            } else {
                pidTarget_ = event->pos().y();
                if (dragEverywhere_) {
                    lastmousePos_ = event->pos().y();
                }
            }

            value_ = (static_cast<float>(pidTarget_ - sliderBlockLength_ / 2) / static_cast<float>(sliderLength_ - sliderBlockLength_) * (maximum_ - minimum_));
            value_ = std::clamp(value_, minimum_, maximum_);
            if (!animationTimer_.isActive()) {
                animationTimer_.start(refreshIntervalMs_);
            }
        } else if (event->button() == Qt::LeftButton && mouseHover_ && mouseEntered_) {
            mousePressed_ = true;

            if (orientation_ == Qt::Horizontal)
                pidTarget_ = event->pos().x();
            else
                pidTarget_ = event->pos().y();
            value_ = (static_cast<float>(pidTarget_ - sliderBlockLength_ / 2) / static_cast<float>(rect().size().height() - sliderBlockLength_) * (maximum_ - minimum_));
            value_ = std::clamp(value_, minimum_, maximum_);
        }
        Extension::mousePressEvent(event);
    };

    bool eventFilter(QObject* obj, QEvent* event) override {
        if (event->type() == QEvent::MouseMove) {
            auto temppos = static_cast<QMouseEvent*>(event)->pos() - pos();
            if (orientation_ == Qt::Horizontal) {
                if (qAbs(temppos.x() - lastpos_) < sliderBlockLength_ / 2 && qAbs(temppos.y() - sliderBlockThickness_ / 2) < sliderBlockThickness_ / 2) {
                    mouseHover_ = true;
                    roundContourWidget_.setRoundContourOpacity(sliderBlockShadowOpacity_);
                } else {
                    mouseHover_ = false;
                    roundContourWidget_.setRoundContourOpacity(0.0);
                }
            } else {
                if (qAbs(temppos.y() - lastpos_) < sliderBlockLength_ / 2 && qAbs(temppos.x() - sliderBlockThickness_ / 2) < sliderBlockThickness_ / 2) {
                    mouseHover_ = true;
                    roundContourWidget_.setRoundContourOpacity(sliderBlockShadowOpacity_);
                } else {
                    mouseHover_ = false;
                    roundContourWidget_.setRoundContourOpacity(0.0);
                }
            }
        }

        if (mousePressed_) {
            if (event->type() == QEvent::MouseMove) {
                mouseHover_ = true;
                roundContourWidget_.setRoundContourOpacity(sliderBlockShadowOpacity_);

                if (orientation_ == Qt::Horizontal)
                    pidTarget_ = static_cast<QMouseEvent*>(event)->pos().x() - pos().x();
                else
                    pidTarget_ = static_cast<QMouseEvent*>(event)->pos().y() - pos().y();
                value_ = (static_cast<float>(pidTarget_ - sliderBlockLength_ / 2) / static_cast<float>(sliderLength_ - sliderBlockLength_) * (maximum_ - minimum_));

                if (value_ < minimum_)
                    value_ = minimum_;
                if (value_ > maximum_)
                    value_ = maximum_;
            }
            if (event->type() == QEvent::MouseButtonRelease) {
                mousePressed_ = false;
                mouseHover_ = false;
                roundContourWidget_.setRoundContourOpacity(0.0);
            }
        }

        if (!animationTimer_.isActive())
            animationTimer_.start(refreshIntervalMs_);

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
