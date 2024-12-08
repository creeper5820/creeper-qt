#include "creeper-qt/widget/list-widget.hh"
#include "creeper-qt/utility/pid.hh"
#include "creeper-qt/widget/switch-button.hh"
#include "creeper-qt/widget/widget.hh"

#include <qdebug.h>
#include <qevent.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qpainterpath.h>
#include <qtimer.h>

using namespace creeper;

struct CustomItemWidgetInterface::Impl {
public:
    inline void backgroundPaintEvent(QPainter& painter, const int& width, const int& height) {
        const auto radius = radiusRatio_ * height;

        auto roundRectPath = QPainterPath();
        if (!isFirst && !isLast) {
            roundRectPath.addRect(0, 0, width, height);
        } else if (isFirst && isLast) {
            roundRectPath.addRoundedRect(0, 0, width, height, radius, radius);
        } else if (isFirst && !isLast) {
            roundRectPath.moveTo(0, radius);
            roundRectPath.lineTo(0, height);
            roundRectPath.lineTo(width, height);
            roundRectPath.lineTo(width, radius);
            roundRectPath.quadTo(width, 0, width - radius, 0);
            roundRectPath.lineTo(radius, 0);
            roundRectPath.quadTo(0, 0, 0, radius);
        } else if (!isFirst && isLast) {
            roundRectPath.lineTo(width, 0);
            roundRectPath.lineTo(width, height - radius);
            roundRectPath.quadTo(width, height, width - radius, height);
            roundRectPath.lineTo(radius, height);
            roundRectPath.quadTo(0, height, 0, height - radius);
            roundRectPath.lineTo(0, 0);
        }

        auto target = mouseHover_ ? mouseHoverOpacity : mouseLeaveOpacity;
        opacity_ = updateWithPid(opacity_, target, 0.1);

        painter.setPen(Qt::NoPen);
        painter.setBrush({ backgroundColor_ });
        painter.setOpacity(opacity_);
        painter.setRenderHint(QPainter::Antialiasing, true);

        painter.drawPath(roundRectPath);
    }

    inline void waterRippleAnimationPaintEvent(
        QPainter& painter, const int& width, const int& height) {
        if (!waterRippleAnimation_) return;

        const auto maxDistance = 2 * std::max(width, height);

        auto roundRectPath = QPainterPath();
        if (!isFirst && !isLast) {
            roundRectPath.addRect(0, 0, width, height);
        } else if (isFirst && isLast) {
            roundRectPath.addRoundedRect(
                0, 0, width, height, radiusRatio_ * height, radiusRatio_ * height);
        } else if (isFirst && !isLast) {
            roundRectPath.moveTo(0, radiusRatio_ * height);
            roundRectPath.lineTo(0, height);
            roundRectPath.lineTo(width, height);
            roundRectPath.lineTo(width, radiusRatio_ * height);
            roundRectPath.quadTo(width, 0, width - radiusRatio_ * height, 0);
            roundRectPath.lineTo(radiusRatio_ * height, 0);
            roundRectPath.quadTo(0, 0, 0, radiusRatio_ * height);
        } else if (!isFirst && isLast) {
            roundRectPath.lineTo(width, 0);
            roundRectPath.lineTo(width, height - radiusRatio_ * height);
            roundRectPath.quadTo(width, height, width - radiusRatio_ * height, height);
            roundRectPath.lineTo(radiusRatio_ * height, height);
            roundRectPath.quadTo(0, height, 0, height - radiusRatio_ * height);
            roundRectPath.lineTo(0, 0);
        }

        painter.setPen(Qt::NoPen);
        painter.setBrush({ waterColor_ });
        painter.setRenderHint(QPainter::Antialiasing, true);

        for (int index = 0; auto& [point, distance] : animationEvents_) {
            painter.setOpacity(0.3 * (1 - static_cast<double>(distance) / maxDistance));

            auto ellipsePath = QPainterPath();
            ellipsePath.addEllipse(point, distance, distance);

            painter.drawPath(ellipsePath.intersected(roundRectPath));

            if (distance > maxDistance) {
                animationEvents_.erase(animationEvents_.begin() + index);
                index--;
            }
            distance += diffusionStep, index++;
        }
    }

    inline void checkAnimation() {
        if (std::abs(opacity_ - (mouseHover_ ? mouseHoverOpacity : mouseLeaveOpacity)) < 0.001
            && animationEvents_.empty())
            animationTimer_.stop();
    }

public:
    // For Animation
    bool selected_ = false;
    bool isFirst = false;
    bool isLast = false;
    bool waterRippleAnimation_ = true;
    bool mouseHover_ = false;

    int diffusionStep = 5;
    double opacity_ = mouseLeaveOpacity;

    double radiusRatio_ = 0.15;

    uint32_t backgroundColor_;
    uint32_t waterColor_;

    QTimer animationTimer_;
    QFont font_;

    std::vector<std::tuple<QPoint, int>> animationEvents_;

    constexpr static inline double mouseHoverOpacity = 1.0;
    constexpr static inline double mouseLeaveOpacity = 0.6;
};

CustomItemWidgetInterface::CustomItemWidgetInterface(QWidget* parent)
    : Extension(parent)
    , pimpl_(new Impl()) {
    reloadTheme();
    connect(&pimpl_->animationTimer_, &QTimer::timeout, [this]() { update(); });
}

CustomItemWidgetInterface::~CustomItemWidgetInterface() { delete pimpl_; }

void CustomItemWidgetInterface::reloadTheme() {
    pimpl_->backgroundColor_ = Theme::color("primary100");
    pimpl_->waterColor_ = Theme::color("primary300");
}

void CustomItemWidgetInterface::setFirst() { pimpl_->isFirst = true; }

void CustomItemWidgetInterface::setLast() { pimpl_->isLast = true; }

void CustomItemWidgetInterface::disableLast() { pimpl_->isLast = false; }

void CustomItemWidgetInterface::paintEvent(QPaintEvent* event) {
    auto painter = QPainter(this);
    const auto width = this->width();
    const auto height = this->height();
    pimpl_->backgroundPaintEvent(painter, width, height);
    pimpl_->waterRippleAnimationPaintEvent(painter, width, height);
    pimpl_->checkAnimation();
}

void CustomItemWidgetInterface::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() & Qt::LeftButton) {
        if (pimpl_->waterRippleAnimation_) {
            pimpl_->animationEvents_.emplace_back(event->pos(), 0);
            if (!pimpl_->animationTimer_.isActive())
                pimpl_->animationTimer_.start(refreshIntervalMs_);
        }
    }
    Extension::mouseReleaseEvent(event);
}

void CustomItemWidgetInterface::enterEvent(QEvent* event) {
    pimpl_->mouseHover_ = true;
    if (!pimpl_->animationTimer_.isActive()) pimpl_->animationTimer_.start(refreshIntervalMs_);
}

void CustomItemWidgetInterface::leaveEvent(QEvent* event) {
    pimpl_->mouseHover_ = false;
    if (!pimpl_->animationTimer_.isActive()) pimpl_->animationTimer_.start(refreshIntervalMs_);
}

struct SwitchAndLabelItem::Impl {
public:
    QLabel* labelWidget_;
    QHBoxLayout* horizonLayout_;
    ConcaveSwitchButton* button_;
};

SwitchAndLabelItem::SwitchAndLabelItem(const QString& label, QWidget* parent)
    : CustomItemWidgetInterface(parent)
    , pimpl_(new Impl()) {
    pimpl_->button_ = new ConcaveSwitchButton;
    pimpl_->button_->setFixedSize({ 50, 30 });

    auto labelWidget_ = new QLabel;
    labelWidget_->setText(label);
    labelWidget_->setFont(QWidget::font());
    labelWidget_->setStyleSheet("color: #575757;");

    pimpl_->horizonLayout_ = new QHBoxLayout;
    pimpl_->horizonLayout_->setAlignment(Qt::AlignLeft);
    pimpl_->horizonLayout_->addLayout(pimpl_->button_->verticalWithSelf());
    pimpl_->horizonLayout_->addWidget(labelWidget_);

    Extension::setLayout(pimpl_->horizonLayout_);
    QListWidgetItem::setSizeHint(QSize(1, 80));
}

SwitchAndLabelItem::~SwitchAndLabelItem() { delete pimpl_; }

void SwitchAndLabelItem::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        pimpl_->button_->setSwitchStatus(!pimpl_->button_->switched());
    }
}

struct ListWidget::Impl {
public:
public:
    int itemNum_ = 0;
};

ListWidget::ListWidget(QWidget* parent)
    : Extension(parent)
    , pimpl_(new Impl()) {
    reloadTheme();
    setFrameShape(Extension::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

ListWidget::~ListWidget() { delete pimpl_; }

void ListWidget::reloadTheme() { }

void ListWidget::addCustomItem(CustomItemWidgetInterface* item) {
    if (!pimpl_->itemNum_) {
        item->setFirst();
        item->setLast();
    } else {
        item->setLast();
        dynamic_cast<CustomItemWidgetInterface*>(this->item(pimpl_->itemNum_ - 1))->disableLast();
    }
    ++(pimpl_->itemNum_);
    Extension::addItem(item);
    Extension::setItemWidget(item, item);
}

void ListWidget::addSwitchAndLabel(const QString& label) {
    auto customItem = new SwitchAndLabelItem(label);
    customItem->QWidget::setFont(font());
    addCustomItem(static_cast<CustomItemWidgetInterface*>(customItem));
}

void ListWidget::addSimpleLabel(const QString& label) {
    auto item = new QListWidgetItem;
    item->setText(label);
    item->setFont(font());
    Extension::addItem(item);
}

void ListWidget::paintEvent(QPaintEvent* event) { }
