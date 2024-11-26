#pragma once

#include "creeper-qt/utility/pid.hh"
#include "creeper-qt/widget/widget.hh"

#include <qapplication.h>
#include <qevent.h>
#include <qpainter.h>
#include <qpoint.h>
#include <qtimer.h>

namespace creeper {

class Slider : public Extension<QWidget> {
public:
    explicit Slider(QWidget* parent = nullptr)
        : Extension(parent) {
        auto app = QCoreApplication::instance();
        app->installEventFilter(this);

        setMouseTracking(true);
        setFixedSize(width(), height());

        connect(&animationTimer_, &QTimer::timeout, [this] { update(); });

        reloadTheme();
    };

    void reloadTheme() override {
        sliderColor_ = Theme::color("primary100");
        roundColor_ = Theme::color("primary300");
    }

    Slider& setOrientation(Qt::Orientation orientation) {
        if (orientation == orientation_) return *this;
        orientation_ = orientation;

        
        if (orientation == Qt::Vertical) setFixedSize(thickness_, length_);
        else if (orientation == Qt::Horizontal)
            setFixedSize(length_, thickness_);

        return *this;
    };

    Qt::Orientation orientation() const { return orientation_; };

    Slider& setMinimum(int minimum) {
        minimum_ = minimum;
        setValue(value_);
        return *this;
    };
    int minimum() const { return minimum_; };

    Slider& setMaximum(int maximum) {
        maximum_ = maximum;
        setValue(value_);
        return *this;
    };
    int maximum() const { return maximum_; };

    Slider& setValue(double value) {
        value_ = std::clamp<double>(value, minimum_, maximum_);
        pidTarget_ = value_ * (length_ - blockLength_) / (maximum_ - minimum_) + blockLength_ / 2.
            + blockBorderShadowSize_;
        if (!animationTimer_.isActive()) animationTimer_.start(10);
        return *this;
    };

    double value() const { return value_; };

    Slider& setFixedSize(int width, int height) {
        if (orientation_ == Qt::Horizontal) {
            Extension::setFixedSize(
                width + 2 * blockBorderShadowSize_, height + 2 * blockBorderShadowSize_);
            length_ = width;
            thickness_ = height;
            lastPos_ = blockLength_ / 2.;
            pidTarget_ = lastPos_;
        } else {
            Extension::setFixedSize(
                width + 2 * blockBorderShadowSize_, height + 2 * blockBorderShadowSize_);
            length_ = height;
            thickness_ = width;
            lastPos_ = blockLength_ / 2.;
            pidTarget_ = lastPos_;
        }
        setValue(value_);
        if (!animationTimer_.isActive()) animationTimer_.start(10);
        return *this;
    }

    Slider& setFixedHeight(int height) {
        setFixedSize(width(), height);
        return *this;
    }

    Slider& setFixedWidth(int width) {
        setFixedSize(width, height());
        return *this;
    }

    Slider& setLength(int length) {
        if (length < blockLength_) blockLength_ = length;
        if (orientation_ == Qt::Horizontal) {
            setFixedSize(length, sliderThickness_);
        } else {
            setFixedSize(sliderThickness_, length);
        }
        return *this;
    }
    int length() const { return length_; }

    Slider& setThickness(int thickness) {
        sliderThickness_ = thickness;
        if (thickness_ < sliderThickness_) {
            if (orientation_ == Qt::Horizontal) setFixedSize(length_, sliderThickness_);
            else
                setFixedSize(sliderThickness_, length_);
        }
        return *this;
    }
    int Thickness() const { return sliderThickness_; }

    Slider& setBLockLength(int length) {
        blockLength_ = length;

        if (blockLength_ > length_) {
            if (orientation_ == Qt::Horizontal) setFixedSize(blockLength_, thickness_);
            else
                setFixedSize(thickness_, blockLength_);
        }
        return *this;
    }
    int bLockLength() const { return blockLength_; }

    Slider& setBLockThickness(int thickness) {
        if (thickness < sliderThickness_) thickness_ = sliderThickness_;
        if (orientation_ == Qt::Horizontal) {
            setFixedSize(length_, thickness);
        } else {
            setFixedSize(thickness, length_);
        }
        return *this;
    }
    int bLockThickness() const { return thickness_; }

    Slider& setBlockBorderShadowSize_(int size) {
        blockBorderShadowSize_ = size;
        update();
        return *this;
    }

    int borderShadowSize_() const { return blockBorderShadowSize_; }

    Slider& setSliderColor(const QColor& color) {
        sliderColor_ = color.value();
        update();
        return *this;
    }
    QColor sliderColor() const { return QColor::fromRgb(sliderColor_); }

    Slider& setRoundColor(const QColor& color) {
        roundColor_ = color.value();
        update();
        return *this;
    }
    QColor roundColor() const { return QColor::fromRgb(roundColor_); }

    Slider& setBlockOpacity(double opacity) {
        sliderOpacity_ = opacity;
        update();
        return *this;
    }
    double blockOpacity() const { return sliderOpacity_; }

    Slider& setBlockShadowOpacity(double opacity) {
        blockShadowOpacity_ = opacity;
        return *this;
    }
    double blockShadowOpacity() const { return blockShadowOpacity_; }

    Slider& setSliderRoundRatio(double ratio) {
        sliderRoundRatio_ = ratio;
        update();
        return *this;
    }
    double sliderRoundRatio() const { return sliderRoundRatio_; }

protected:
    void paintEvent(QPaintEvent* event) override {
        auto painter = QPainter(this);
        sliderPaintEvent(painter);
        animationPaintEvent(painter);
        shadowPaintEvent(painter);
        checkAnimation(painter);
    };

    void sliderPaintEvent(QPainter& painter) {
        const auto size = rect().size();

        painter.setPen(Qt::NoPen);
        painter.setBrush({ sliderColor_ });
        painter.setOpacity(sliderBackgroundOpacity_);
        painter.setRenderHint(QPainter::Antialiasing, true);
        if (orientation_ == Qt::Horizontal)
            painter.drawRoundedRect(blockBorderShadowSize_,
                (thickness_ - sliderThickness_) / 2 + blockBorderShadowSize_, length_,
                sliderThickness_, sliderThickness_ / 2. * sliderRoundRatio_,
                sliderThickness_ / 2. * sliderRoundRatio_);
        else
            painter.drawRoundedRect((thickness_ - sliderThickness_) / 2 + blockBorderShadowSize_,
                blockBorderShadowSize_, sliderThickness_, length_,
                sliderThickness_ / 2. * sliderRoundRatio_,
                sliderThickness_ / 2. * sliderRoundRatio_);
    }

    void animationPaintEvent(QPainter& painter) {
        const auto halfblockLength = blockLength_ / 2.;
        lastPos_ = std::clamp(updateWithPid(lastPos_, pidTarget_, 0.1),
            halfblockLength + blockBorderShadowSize_,
            length_ - halfblockLength + blockBorderShadowSize_);

        painter.setOpacity(sliderOpacity_);
        if (orientation_ == Qt::Horizontal) {
            painter.drawRoundedRect(blockBorderShadowSize_,
                (thickness_ - sliderThickness_) / 2 + blockBorderShadowSize_,
                static_cast<int>(lastPos_ + halfblockLength) - blockBorderShadowSize_,
                sliderThickness_, sliderThickness_ / 2. * sliderRoundRatio_,
                sliderThickness_ / 2. * sliderRoundRatio_);

            painter.setBrush({ roundColor_ });
            painter.setOpacity(sliderOpacity_);
            painter.drawRoundedRect(static_cast<int>(lastPos_ - halfblockLength),
                blockBorderShadowSize_, blockLength_, thickness_,
                thickness_ / 2. * sliderRoundRatio_, thickness_ / 2. * sliderRoundRatio_);
        } else {
            painter.drawRoundedRect((thickness_ - sliderThickness_) / 2 + blockBorderShadowSize_,
                blockBorderShadowSize_, sliderThickness_,
                static_cast<int>(lastPos_ + halfblockLength) - blockBorderShadowSize_,
                sliderThickness_ / 2. * sliderRoundRatio_,
                sliderThickness_ / 2. * sliderRoundRatio_);

            painter.setBrush({ roundColor_ });
            painter.setOpacity(sliderOpacity_);
            painter.drawRoundedRect(blockBorderShadowSize_,
                static_cast<int>(lastPos_ - halfblockLength), thickness_, blockLength_,
                thickness_ / 2. * sliderRoundRatio_, thickness_ / 2. * sliderRoundRatio_);
        }
    }

    void shadowPaintEvent(QPainter& painter) {
        const auto halfblockLength = blockLength_ / 2.;
        lastShadowOpacity_ = updateWithPid(lastShadowOpacity_, shadowOpacityPidTarget_, 0.03);
        painter.setOpacity(lastShadowOpacity_);
        if (orientation_ == Qt::Horizontal) {
            painter.drawRoundedRect(
                static_cast<int>(lastPos_ - halfblockLength) - blockBorderShadowSize_, 0,
                blockLength_ + 2 * blockBorderShadowSize_, thickness_ + 2 * blockBorderShadowSize_,
                thickness_ / 2. + blockBorderShadowSize_ * sliderRoundRatio_,
                thickness_ / 2. + blockBorderShadowSize_ * sliderRoundRatio_);
        } else {
            painter.drawRoundedRect(0,
                static_cast<int>(lastPos_ - halfblockLength) - blockBorderShadowSize_,
                thickness_ + 2 * blockBorderShadowSize_, blockLength_ + 2 * blockBorderShadowSize_,
                thickness_ / 2. + blockBorderShadowSize_ * sliderRoundRatio_,
                thickness_ / 2. + blockBorderShadowSize_ * sliderRoundRatio_);
        }
    }

    void checkAnimation(QPainter& painter) {
        const auto halfblockLength = blockLength_ / 2.;
        if (std::abs(lastPos_
                - ((value_ - minimum_) * (length_ - blockLength_) / (maximum_ - minimum_)
                    + halfblockLength + blockBorderShadowSize_))
                <= 1.
            && std::abs(lastShadowOpacity_ - shadowOpacityPidTarget_) <= 0.001) {
            animationTimer_.stop();
        }
    }

    void mousePressEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton && !mouseHover_ && mouseEntered_) {
            mouseHover_ = true;
            shadowOpacityPidTarget_ = blockShadowOpacity_;
            mousePressed_ = true;

            if (orientation_ == Qt::Horizontal) {
                pidTarget_ = event->pos().x();
            } else {
                pidTarget_ = event->pos().y();
            }

            value_ = (pidTarget_ - blockLength_ / 2. - blockBorderShadowSize_)
                    * (maximum_ - minimum_) / (length_ - blockLength_)
                + minimum_;
            value_ = std::clamp<double>(value_, minimum_, maximum_);
            if (!animationTimer_.isActive()) {
                animationTimer_.start(refreshIntervalMs_);
            }
        } else if (event->button() == Qt::LeftButton && mouseHover_ && mouseEntered_) {
            mousePressed_ = true;

            if (orientation_ == Qt::Horizontal) pidTarget_ = event->pos().x();
            else
                pidTarget_ = event->pos().y();
            value_ = (pidTarget_ - blockLength_ / 2. - blockBorderShadowSize_)
                    * (maximum_ - minimum_) / (length_ - blockLength_)
                + minimum_;
            value_ = std::clamp<double>(value_, minimum_, maximum_);
        }
        Extension::mousePressEvent(event);
    };

    void mouseMoveEvent(QMouseEvent* event) override {
        if (mouseEntered_) {
            if (orientation_ == Qt::Horizontal) {
                if (std::abs(event->pos().x() - lastPos_) <= blockLength_
                    && std::abs(event->pos().y() - thickness_ / 2 - blockBorderShadowSize_)
                        <= thickness_) {
                    mouseHover_ = true;
                    shadowOpacityPidTarget_ = blockShadowOpacity_;
                } else {
                    mouseHover_ = false;
                    shadowOpacityPidTarget_ = 0.;
                }
            } else {
                if (std::abs(event->pos().y() - lastPos_) <= blockLength_
                    && std::abs(event->pos().x() - thickness_ / 2 - blockBorderShadowSize_)
                        <= thickness_) {
                    mouseHover_ = true;
                    shadowOpacityPidTarget_ = blockShadowOpacity_;
                } else {
                    mouseHover_ = false;
                    shadowOpacityPidTarget_ = 0.;
                }
            }
            if (!animationTimer_.isActive()) animationTimer_.start(refreshIntervalMs_);
        }
        Extension::mouseMoveEvent(event);
    }

    bool eventFilter(QObject* obj, QEvent* event) override {
        if (mouseEntered_ && mousePressed_) {
            if (event->type() == QEvent::MouseMove) {
                mouseHover_ = true;

                if (orientation_ == Qt::Horizontal)
                    pidTarget_ = dynamic_cast<QMouseEvent*>(event)->pos().x() - pos().x();
                else
                    pidTarget_ = dynamic_cast<QMouseEvent*>(event)->pos().y() - pos().y();
                value_ = (pidTarget_ - blockLength_ / 2. - blockBorderShadowSize_)
                        * (maximum_ - minimum_) / (length_ - blockLength_)
                    + minimum_;

                value_ = std::clamp<double>(value_, minimum_, maximum_);

                shadowOpacityPidTarget_ = blockShadowOpacity_;
                if (!animationTimer_.isActive()) animationTimer_.start(refreshIntervalMs_);
            }
            if (event->type() == QEvent::MouseButtonRelease) {
                mousePressed_ = false;
                mouseHover_ = false;
                shadowOpacityPidTarget_ = 0.0;

                if (!animationTimer_.isActive()) animationTimer_.start(refreshIntervalMs_);
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
        shadowOpacityPidTarget_ = 0.0;
        if (!animationTimer_.isActive()) animationTimer_.start(refreshIntervalMs_);
        Extension::leaveEvent(event);
    }

private:
    bool mouseHover_ = false;
    bool mousePressed_ = false;
    bool mouseEntered_ = false;

    QTimer animationTimer_;

    QPointF center;

    uint32_t sliderColor_;
    uint32_t roundColor_;

    double sliderRoundRatio_ = 0.8;

    int length_ = 200;
    int sliderThickness_ = 20;
    int blockLength_ = 40;
    int thickness_ = 20;
    int blockBorderShadowSize_ = 5;

    double pidTarget_;
    double lastPos_;

    double shadowOpacityPidTarget_ = 0;
    double lastShadowOpacity_ = 0;

    double sliderBackgroundOpacity_ = 0.2;
    double sliderOpacity_ = 1.0;
    double blockShadowOpacity_ = 0.5;

    Qt::Orientation orientation_ = Qt::Horizontal;
    int minimum_ = 0;
    int maximum_ = 100;
    double value_ = 0;
};
}
