#include "creeper-qt/widget/convex-slider.hh"
#include "creeper-qt/utility/pid.hh"

#include <qapplication.h>
#include <qpainter.h>
#include <qpoint.h>
#include <qtimer.h>
#include <qwidget.h>

using namespace creeper;

struct ConvexSlider::Impl {
public:
    inline void ratioToNumeric(int width, int height) {
        const auto roundSize = sliderThickness_ / 2. * sliderRoundRatio_;

        if (width >= height) {
            blockThickness_ = static_cast<int>(height * blockThicknessRatio_);
            blockShadowBorderSize_ = static_cast<int>((height - blockThickness_) / 2.);
            sliderLength_ = static_cast<int>((width - 2 * blockShadowBorderSize_) * sliderSizeRatio_
                - blockThickness_ * (blockLengthRatio_ - 1));
            sliderThickness_
                = static_cast<int>((height - 2 * blockShadowBorderSize_) * sliderSizeRatio_);
            lastPos_ = width / 2. - sliderLength_ / 2. + roundSize;
        } else {
            blockThickness_ = static_cast<int>(width * blockThicknessRatio_);
            blockShadowBorderSize_ = static_cast<int>((width - blockThickness_) / 2.);
            sliderLength_
                = static_cast<int>((height - 2 * blockShadowBorderSize_) * sliderSizeRatio_
                    - blockThickness_ * (blockLengthRatio_ - 1));
            sliderThickness_
                = static_cast<int>((width - 2 * blockShadowBorderSize_) * sliderSizeRatio_);
            lastPos_ = height / 2. - sliderLength_ / 2. + roundSize;
        }
    }

    inline void sliderPaintEvent(QPainter& painter, int width, int height) {
        painter.setPen(Qt::NoPen);
        painter.setRenderHint(QPainter::Antialiasing, true);

        double x0, y0;
        if (width >= height) {
            x0 = width / 2. - sliderLength_ / 2.;
            y0 = height / 2. - sliderThickness_ / 2.;
        } else {
            x0 = height / 2. - sliderLength_ / 2.;
            y0 = width / 2. - sliderThickness_ / 2.;
        }
        const auto roundSize = sliderThickness_ / 2. * sliderRoundRatio_;
        // 绘制背景
        painter.setBrush({ sliderColor_ });
        painter.setOpacity(backgroundOpacity_);
        painter.drawRoundedRect(static_cast<int>(x0), static_cast<int>(y0), sliderLength_,
            sliderThickness_, roundSize, roundSize);

        const auto minPos = x0 + roundSize;
        const auto maxPos = x0 + sliderLength_ - roundSize;
        const auto halfblockLength = blockThickness_ * blockLengthRatio_ / 2.;
        lastPos_ = std::clamp(updateWithPid(lastPos_, pidTarget_, 0.1), minPos, maxPos);
        // 绘制条
        painter.setOpacity(sliderOpacity_);
        painter.drawRoundedRect(static_cast<int>(x0), static_cast<int>(y0),
            static_cast<int>(lastPos_ - x0 + halfblockLength), sliderThickness_, roundSize,
            roundSize);

        const auto blockRoundSize = blockThickness_ / 2. * sliderRoundRatio_;
        // 绘制滑块
        painter.setBrush({ roundColor_ });
        painter.drawRoundedRect(static_cast<int>(lastPos_ - halfblockLength),
            blockShadowBorderSize_, static_cast<int>(blockThickness_ * blockLengthRatio_),
            blockThickness_, blockRoundSize, blockRoundSize);

        const auto shadowLength = blockThickness_ * blockLengthRatio_ + 2 * blockShadowBorderSize_;
        const auto shadowThickness = blockThickness_ + 2 * blockShadowBorderSize_;
        const auto shadowRoundSize
            = blockThickness_ / 2. + blockShadowBorderSize_ * sliderRoundRatio_;
        lastShadowOpacity_ = updateWithPid(lastShadowOpacity_, shadowOpacityPidTarget_, 0.03);
        // 绘制滑块阴影
        painter.setOpacity(lastShadowOpacity_);
        painter.drawRoundedRect(
            static_cast<int>(lastPos_ - halfblockLength) - blockShadowBorderSize_, 0,
            static_cast<int>(shadowLength), shadowThickness, shadowRoundSize, shadowRoundSize);

        const auto valuePos = (value_ - minimum_)
                * (sliderLength_ - sliderThickness_ * sliderRoundRatio_) / (maximum_ - minimum_)
            + minPos;
        // 停止动画
        if (std::abs(lastPos_ - valuePos) <= 1.
            && std::abs(lastShadowOpacity_ - shadowOpacityPidTarget_) <= 0.001) {
            animationTimer_.stop();
        }
    }

public:
    bool mouseHover_ = false;
    bool mousePressed_ = false;
    bool mouseEntered_ = false;

    QTimer animationTimer_;

    uint32_t sliderColor_;
    uint32_t roundColor_;

    double sliderSizeRatio_ = 1.;
    double sliderRoundRatio_ = 1.;
    double blockThicknessRatio_ = 0.5;
    double blockLengthRatio_ = 1.;

    int sliderLength_;
    int sliderThickness_;
    int blockThickness_;
    int blockShadowBorderSize_ = 5;

    double pidTarget_;
    double lastPos_;
    double shadowOpacityPidTarget_ = 0;
    double lastShadowOpacity_ = 0;

    double backgroundOpacity_ = 0.2;
    double sliderOpacity_ = 1.0;
    double shadowOpacity_ = 0.5;

    double minimum_ = 0;
    double maximum_ = 100;
    double value_ = 0;
};

void ConvexSlider::reloadTheme() {
    pimpl_->sliderColor_ = Theme::color("primary200");
    pimpl_->roundColor_ = Theme::color("primary700");
}

ConvexSlider::ConvexSlider(QWidget* parent)
    : Extension(parent)
    , pimpl_(new Impl()) {
    auto app = QCoreApplication::instance();
    app->installEventFilter(this);
    reloadTheme();
    setMouseTracking(true);

    connect(&pimpl_->animationTimer_, &QTimer::timeout, [this] { update(); });
}

ConvexSlider::~ConvexSlider() { delete pimpl_; }

ConvexSlider& ConvexSlider::setRange(double minimum, double maximum) {
    setMinimum(minimum).setMaximum(maximum);
    return *this;
}

ConvexSlider& ConvexSlider::setMinimum(double minimum) {
    if (minimum > pimpl_->maximum_) return *this;
    pimpl_->minimum_ = minimum;
    setValue(std::clamp(pimpl_->value_, pimpl_->minimum_, pimpl_->maximum_));
    return *this;
}
double ConvexSlider::minimum() const { return pimpl_->minimum_; }

ConvexSlider& ConvexSlider::setMaximum(double maximum) {
    if (maximum < pimpl_->minimum_) return *this;
    pimpl_->maximum_ = maximum;
    setValue(std::clamp(pimpl_->value_, pimpl_->minimum_, pimpl_->maximum_));
    return *this;
}
double ConvexSlider::maximum() const { return pimpl_->maximum_; }

ConvexSlider& ConvexSlider::setValue(double value) {
    const auto roundSize = pimpl_->sliderThickness_ / 2. * pimpl_->sliderRoundRatio_;
    double minPos;
    if (width() >= height()) minPos = width() / 2. - pimpl_->sliderLength_ / 2. + roundSize;
    else minPos = height() / 2. - pimpl_->sliderLength_ / 2. + roundSize;

    pimpl_->value_ = std::clamp<double>(value, pimpl_->minimum_, pimpl_->maximum_);
    pimpl_->pidTarget_ = (pimpl_->value_ - pimpl_->minimum_) * (pimpl_->sliderLength_ - roundSize)
            / (pimpl_->maximum_ - pimpl_->minimum_)
        + minPos;
    if (!pimpl_->animationTimer_.isActive()) pimpl_->animationTimer_.start(refreshIntervalMs_);
    return *this;
}
double ConvexSlider::value() const { return pimpl_->value_; }

ConvexSlider& ConvexSlider::resize(int width, int height) {
    pimpl_->ratioToNumeric(width, height);
    const auto roundSize = pimpl_->sliderThickness_ / 2. * pimpl_->sliderRoundRatio_;
    double minPos;
    if (width >= height) {
        minPos = width / 2. - pimpl_->sliderLength_ / 2. + roundSize;
    } else {
        minPos = height / 2. - pimpl_->sliderLength_ / 2. + roundSize;
    }
    pimpl_->lastPos_ = minPos;
    setValue(pimpl_->value_);
    Extension::resize(width, height);
    return *this;
}

ConvexSlider& ConvexSlider::setFixedSize(QSize size) {
    setFixedSize(size.width(), size.height());
    return *this;
}

ConvexSlider& ConvexSlider::setFixedSize(int width, int height) {
    pimpl_->ratioToNumeric(width, height);
    const auto roundSize = pimpl_->sliderThickness_ / 2. * pimpl_->sliderRoundRatio_;
    double minPos;
    if (width >= height) {
        minPos = width / 2. - pimpl_->sliderLength_ / 2. + roundSize;
    } else {
        minPos = height / 2. - pimpl_->sliderLength_ / 2. + roundSize;
    }
    pimpl_->lastPos_ = minPos;
    setValue(pimpl_->value_);
    Extension::setFixedSize(width, height);
    return *this;
}

ConvexSlider& ConvexSlider::setFixedHeight(int height) {
    Extension::setFixedHeight(height);
    return *this;
}

ConvexSlider& ConvexSlider::setFixedWidth(int width) {
    Extension::setFixedHeight(width);
    return *this;
}

ConvexSlider& ConvexSlider::setBlockThicknessRatio(double blockThicknessRatio) {
    if (width() >= height()) {
        pimpl_->sliderThickness_ = static_cast<int>(height() * pimpl_->blockThicknessRatio_);
        if (height() * blockThicknessRatio < pimpl_->sliderThickness_) {
            blockThicknessRatio = height() / static_cast<double>(pimpl_->sliderThickness_);
            return *this;
        }
    } else {
        pimpl_->sliderThickness_ = static_cast<int>(width() * pimpl_->blockThicknessRatio_);
        if (width() * blockThicknessRatio < pimpl_->sliderThickness_) {
            blockThicknessRatio = width() / static_cast<double>(pimpl_->sliderThickness_);
            return *this;
        }
    }
    pimpl_->blockThicknessRatio_ = blockThicknessRatio;
    update();
    return *this;
};
double ConvexSlider::blockThicknessRatio() const { return pimpl_->blockThicknessRatio_; };

ConvexSlider& ConvexSlider::setBlockLengthRatio(double ratio) {
    if (ratio <= 1) return *this;
    if (width() >= height()) {
        if (pimpl_->blockThickness_ * ratio + pimpl_->blockShadowBorderSize_ * 2 > width())
            return *this;
    } else {
        if (pimpl_->blockThickness_ * ratio + pimpl_->blockShadowBorderSize_ * 2 > height())
            return *this;
    }

    pimpl_->blockLengthRatio_ = ratio;
    update();
    return *this;
};
double ConvexSlider::blockLengthRatio() const { return pimpl_->blockLengthRatio_; };

ConvexSlider& ConvexSlider::setSliderSizeRatio(double ratio) {
    pimpl_->sliderSizeRatio_ = ratio;
    update();
    return *this;
};
double ConvexSlider::sliderSizeRatio() const { return pimpl_->sliderSizeRatio_; };

ConvexSlider& ConvexSlider::setSliderRoundRatio(double ratio) {
    pimpl_->sliderRoundRatio_ = ratio;
    update();
    return *this;
};
double ConvexSlider::sliderRoundRatio() const { return pimpl_->sliderRoundRatio_; };

ConvexSlider& ConvexSlider::setSliderColor(const QColor& color) {
    pimpl_->sliderColor_ = color.value();
    update();
    return *this;
};
QColor ConvexSlider::sliderColor() const { return QColor::fromRgb(pimpl_->sliderColor_); };

ConvexSlider& ConvexSlider::setRoundColor(const QColor& color) {
    pimpl_->roundColor_ = color.value();
    update();
    return *this;
};
QColor ConvexSlider::roundColor() const { return QColor::fromRgb(pimpl_->roundColor_); };

ConvexSlider& ConvexSlider::setBackgroundOpacity(double opacity) {
    pimpl_->backgroundOpacity_ = opacity;
    update();
    return *this;
};
double ConvexSlider::backgroundOpacity() const { return pimpl_->backgroundOpacity_; };
ConvexSlider& ConvexSlider::setBlockOpacity(double opacity) {
    pimpl_->sliderOpacity_ = opacity;
    update();
    return *this;
};
double ConvexSlider::blockOpacity() const { return pimpl_->sliderOpacity_; };

ConvexSlider& ConvexSlider::setBlockShadowOpacity(double opacity) {
    pimpl_->backgroundOpacity_ = opacity;
    update();
    return *this;
};
double ConvexSlider::blockShadowOpacity() const { return pimpl_->backgroundOpacity_; };

void ConvexSlider::paintEvent(QPaintEvent* event) {
    auto painter = QPainter(this);
    if (height() >= width()) {
        painter.translate(width(), 0);
        painter.rotate(90);
    }
    pimpl_->sliderPaintEvent(painter, width(), height());
};

bool ConvexSlider::eventFilter(QObject* obj, QEvent* event) {
    if (pimpl_->mousePressed_) {
        if (event->type() == QEvent::MouseMove) {
            const auto roundSize = pimpl_->sliderThickness_ / 2. * pimpl_->sliderRoundRatio_;
            double minPos;

            if (width() >= height()) {
                minPos = width() / 2. - pimpl_->sliderLength_ / 2. + roundSize;
                pimpl_->pidTarget_ = dynamic_cast<QMouseEvent*>(event)->pos().x() - pos().x();
            } else {
                minPos = height() / 2. - pimpl_->sliderLength_ / 2. + roundSize;
                pimpl_->pidTarget_ = dynamic_cast<QMouseEvent*>(event)->pos().y() - pos().y();
            }
            const auto maxPos = minPos + pimpl_->sliderLength_ - 2 * roundSize;

            pimpl_->pidTarget_ = std::clamp(pimpl_->pidTarget_, minPos, maxPos);
            pimpl_->value_ = (pimpl_->pidTarget_ - minPos) * (pimpl_->maximum_ - pimpl_->minimum_)
                    / (pimpl_->sliderLength_ - 2 * roundSize)
                + pimpl_->minimum_;

            pimpl_->shadowOpacityPidTarget_ = pimpl_->shadowOpacity_;
            if (!pimpl_->animationTimer_.isActive())
                pimpl_->animationTimer_.start(refreshIntervalMs_);
        }
        if (event->type() == QEvent::MouseButtonRelease) {
            pimpl_->mousePressed_ = false;
            pimpl_->mouseHover_ = false;
            pimpl_->shadowOpacityPidTarget_ = 0.0;

            if (!pimpl_->animationTimer_.isActive())
                pimpl_->animationTimer_.start(refreshIntervalMs_);
        }
    }

    return Extension::eventFilter(obj, event);
};

void ConvexSlider::mouseMoveEvent(QMouseEvent* event) {
    if (pimpl_->mouseEntered_) {
        int pos0, pos1;
        if (width() >= height()) {
            pos0 = event->pos().x();
            pos1 = event->pos().y();
        } else {
            pos0 = event->pos().y();
            pos1 = event->pos().x();
        }
        if (std::abs(pos0 - pimpl_->lastPos_) <= pimpl_->blockThickness_ * pimpl_->blockLengthRatio_
            && std::abs(pos1 - pimpl_->blockThickness_ / 2 - pimpl_->blockShadowBorderSize_)
                <= pimpl_->blockThickness_) {
            pimpl_->mouseHover_ = true;
            pimpl_->shadowOpacityPidTarget_ = pimpl_->shadowOpacity_;
        } else {
            pimpl_->mouseHover_ = false;
            pimpl_->shadowOpacityPidTarget_ = 0.;
        }
        if (!pimpl_->animationTimer_.isActive()) pimpl_->animationTimer_.start(refreshIntervalMs_);
    }
    Extension::mouseMoveEvent(event);
};

void ConvexSlider::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && !pimpl_->mouseHover_) {
        pimpl_->mouseHover_ = true;
        pimpl_->shadowOpacityPidTarget_ = pimpl_->shadowOpacity_;
        pimpl_->mousePressed_ = true;
    } else if (event->button() == Qt::LeftButton && pimpl_->mouseHover_)
        pimpl_->mousePressed_ = true;

    if (pimpl_->mousePressed_) {
        const auto roundSize = pimpl_->sliderThickness_ / 2. * pimpl_->sliderRoundRatio_;
        double minPos;
        if (width() >= height()) {
            minPos = width() / 2. - pimpl_->sliderLength_ / 2. + roundSize;
            pimpl_->pidTarget_ = event->pos().x();
        } else {
            minPos = height() / 2. - pimpl_->sliderLength_ / 2. + roundSize;
            pimpl_->pidTarget_ = event->pos().y();
        }
        const auto maxPos = minPos + pimpl_->sliderLength_ - 2 * roundSize;

        pimpl_->pidTarget_ = std::clamp(pimpl_->pidTarget_, minPos, maxPos);
        pimpl_->value_ = (pimpl_->pidTarget_ - minPos) * (pimpl_->maximum_ - pimpl_->minimum_)
                / (pimpl_->sliderLength_ - 2 * roundSize)
            + pimpl_->minimum_;
    }
    Extension::mousePressEvent(event);
};

void ConvexSlider::resizeEvent(QResizeEvent* event) {
    Extension::resizeEvent(event);
    pimpl_->ratioToNumeric(event->size().width(), event->size().height());
    setValue(pimpl_->value_);
    if (!pimpl_->animationTimer_.isActive()) pimpl_->animationTimer_.start(refreshIntervalMs_);
};

void ConvexSlider::enterEvent(QEvent* event) {
    pimpl_->mouseEntered_ = true;
    Extension::enterEvent(event);
};

void ConvexSlider::leaveEvent(QEvent* event) {
    pimpl_->mouseEntered_ = false;
    pimpl_->mouseHover_ = false;
    pimpl_->shadowOpacityPidTarget_ = 0.;
    Extension::leaveEvent(event);
};
