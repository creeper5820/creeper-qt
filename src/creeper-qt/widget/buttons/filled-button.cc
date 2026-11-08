#include "filled-button.hh"

#include "creeper-qt/details/drawable/core.hh"
#include "creeper-qt/details/drawable/shape/rect.hh"
#include "creeper-qt/details/drawable/shape/text.hh"
#include "creeper-qt/utility/animation/math.hh"
#include "creeper-qt/utility/animation/state/pid.hh"
#include "creeper-qt/utility/animation/transition.hh"
#include "creeper-qt/utility/animation/water-ripple.hh"
#include "creeper-qt/utility/theme/theme.hh"

#include <qevent.h>

namespace creeper {

struct FilledButton::Impl {
public:
    bool enable_water_ripple = true;

    double radius     = -1;
    QColor text_color = Qt::black;
    QColor background = Qt::white;

    QColor border_color = Qt::red;
    double border_width = 0;

    QColor water_color = Qt::black;

    QColor kHoverColor  = QColor { 0, 0, 0, 30 };
    bool is_mouse_hover = false;

    Animatable animatable;
    WaterRipple water_ripple;

    std::unique_ptr<TransitionValue<PidState<Eigen::Vector4d>>> hover_color;

public:
    explicit Impl(QAbstractButton& self)
        : animatable { self }
        , water_ripple { animatable } {
        {
            auto state = std::make_shared<PidState<Eigen::Vector4d>>();

            state->config.kp = 20;
            state->config.ki = 0;
            state->config.kd = 0;

            hover_color = make_transition(animatable, std::move(state));
        }

        const auto radius = std::min<double>(self.rect().height(), self.rect().width()) / 2;

        water_ripple.setSize(self.size());
        water_ripple.setClipPath(make_rounded_rectangle_path(self.rect(), radius));
        water_ripple.setColor(water_color);
    }

    void paint_event(QAbstractButton& self, QPaintEvent* event) {
        using namespace drawable;

        const auto radius = this->radius < 0
            ? std::min<double>(self.rect().height(), self.rect().width()) / 2
            : this->radius;

        const auto hover_color = from_vector4(*this->hover_color);

        auto painter = QPainter { &self };
        DrawHere {
            painter,
            Rect {
              .size  = self.size(),
              .color = background,

              .outline_color = border_color,
              .outline_width = border_width,

              .radius_tl = radius,
              .radius_tr = radius,
              .radius_br = radius,
              .radius_bl = radius,
            },
            water_ripple,
            Rect {
              .size  = self.size(),
              .color = hover_color,

              .radius_tl = radius,
              .radius_tr = radius,
              .radius_br = radius,
              .radius_bl = radius,
            },
            Text {
              .text  = self.text(),
              .font  = self.font(),
              .size  = self.size(),
              .color = text_color,
            },
        };
    }

    void mouse_press_event(QAbstractButton& self, QMouseEvent* event) {
        if (enable_water_ripple) water_ripple.press(event->pos());
    }

    void mouse_release_event() {
        water_ripple.release();
    }

    void enter_event(QAbstractButton& self, qt::EnterEvent* event) {
        hover_color->transition_to(from_color(kHoverColor));
        is_mouse_hover = true;
    }

    void leave_event(QAbstractButton& self, QEvent* event) {
        hover_color->transition_to(from_color(Qt::transparent));
        is_mouse_hover = false;
    }

private:
    static QPainterPath make_rounded_rectangle_path(const QRectF& rect, double radius) {
        auto path = QPainterPath { };
        path.addRoundedRect(rect, radius, radius);
        return path;
    }
};

FilledButton::FilledButton()
    : pimpl(std::make_unique<Impl>(*this)) { }

FilledButton::~FilledButton() = default;

void FilledButton::loadColorScheme(const ColorScheme& pack) {
    pimpl->background = pack.primary;
    pimpl->text_color = pack.on_primary;

    if (pack.primary.lightness() > 128) {
        pimpl->water_color = pack.primary.darker(130);
        pimpl->kHoverColor = QColor { 000, 000, 000, 30 };
    } else {
        pimpl->water_color = pack.primary.lighter(130);
        pimpl->kHoverColor = QColor { 255, 255, 255, 30 };
    }

    pimpl->water_ripple.setColor(pimpl->water_color);

    update();
}

void FilledButton::bindThemeManager(ThemeManager& manager) {
    manager.appendHandler(this, [this](const ThemeManager& manager) {
        const auto color_mode   = manager.colorMode();
        const auto theme_pack   = manager.themePack();
        const auto color_scheme = color_mode == ColorMode::LIGHT //
            ? theme_pack.light
            : theme_pack.dark;
        loadColorScheme(color_scheme);
    });
}

// 属性设置接口实现

void FilledButton::setRadius(double radius) {
    pimpl->radius = radius;

    const auto effective_radius = radius < 0 ? std::min<double>(width(), height()) / 2 : radius;
    auto path                   = QPainterPath { };
    path.addRoundedRect(rect(), effective_radius, effective_radius);

    pimpl->water_ripple.setSize(size());
    pimpl->water_ripple.setClipPath(path);
}

void FilledButton::setBorderWidth(double border) {
    pimpl->border_width = border;
}

void FilledButton::setBorderColor(const QColor& color) {
    pimpl->border_color = color;
}

void FilledButton::setWaterColor(const QColor& color) {
    pimpl->water_color = color;
    pimpl->water_ripple.setColor(color);
}

void FilledButton::setTextColor(const QColor& color) {
    pimpl->text_color = color;
}

void FilledButton::setBackground(const QColor& color) {
    pimpl->background = color;
}

void FilledButton::setHoverColor(const QColor& color) {
    pimpl->kHoverColor = color;
}

void FilledButton::setWaterRippleStatus(bool enable) {
    pimpl->enable_water_ripple = enable;
}

// Qt 接口重载

void FilledButton::mousePressEvent(QMouseEvent* event) {
    pimpl->mouse_press_event(*this, event);
    QAbstractButton::mousePressEvent(event);
}
void FilledButton::mouseReleaseEvent(QMouseEvent* event) {
    pimpl->mouse_release_event();
    QAbstractButton::mouseReleaseEvent(event);
}
void FilledButton::paintEvent(QPaintEvent* event) {
    pimpl->paint_event(*this, event);
    /* Disable QAbstractButton::paintEvent */;
}
void FilledButton::resizeEvent(QResizeEvent* event) {
    const auto radius = pimpl->radius < 0 ? std::min<double>(width(), height()) / 2 : pimpl->radius;
    auto path         = QPainterPath { };
    path.addRoundedRect(rect(), radius, radius);

    pimpl->water_ripple.setSize(size());
    pimpl->water_ripple.setClipPath(path);

    QAbstractButton::resizeEvent(event);
}
void FilledButton::enterEvent(qt::EnterEvent* event) {
    pimpl->enter_event(*this, event);
    QAbstractButton::enterEvent(event);
}
void FilledButton::leaveEvent(QEvent* event) {
    pimpl->leave_event(*this, event);
    QAbstractButton::leaveEvent(event);
}

}
