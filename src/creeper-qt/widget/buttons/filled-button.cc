#include "filled-button.hh"

#include "creeper-qt/utility/animation/math.hh"
#include "creeper-qt/utility/animation/state/pid.hh"
#include "creeper-qt/utility/animation/transition.hh"
#include "creeper-qt/utility/animation/water-ripple.hh"
#include "creeper-qt/utility/painter/helper.hh"
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
    WaterRippleRenderer water_ripple;

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
    }

    void paint_event(QAbstractButton& self, QPaintEvent* event) {

        const auto radius = this->radius < 0
            ? std::min<double>(self.rect().height(), self.rect().width()) / 2
            : this->radius;

        const auto button_path = make_rounded_rectangle_path(self.rect(), radius);
        const auto hover_color = from_vector4(*this->hover_color);

        auto painter = QPainter { &self };
        util::PainterHelper { painter }
            .set_render_hint(QPainter::RenderHint::Antialiasing)
            .set_opacity(1.)
            .rounded_rectangle(background, border_color, border_width, self.rect(), radius, radius)
            .apply(water_ripple.renderer(button_path, water_color))
            .set_opacity(1.)
            .rounded_rectangle(hover_color, Qt::transparent, 0, self.rect(), radius, radius)
            .set_opacity(1.)
            .simple_text(self.text(), self.font(), text_color, self.rect(), Qt::AlignCenter)
            .done();
    }

    void mouse_press_event(QAbstractButton& self, QMouseEvent* event) {
        if (enable_water_ripple) {
            const auto center_point = event->pos();
            const auto max_distance = std::max<double>(self.width(), self.height());
            water_ripple.press(center_point, max_distance);
        }
    }

    void mouse_release_event() { water_ripple.release(); }

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
    pimpl->water_color.setAlphaF(0.4);

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

void FilledButton::setRadius(double radius) { pimpl->radius = radius; }

void FilledButton::setBorderWidth(double border) { pimpl->border_width = border; }

void FilledButton::setBorderColor(const QColor& color) { pimpl->border_color = color; }

void FilledButton::setWaterColor(const QColor& color) { pimpl->water_color = color; }

void FilledButton::setTextColor(const QColor& color) { pimpl->text_color = color; }

void FilledButton::setBackground(const QColor& color) { pimpl->background = color; }

void FilledButton::setHoverColor(const QColor& color) { pimpl->kHoverColor = color; }

void FilledButton::setWaterRippleStatus(bool enable) { pimpl->enable_water_ripple = enable; }

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
void FilledButton::enterEvent(qt::EnterEvent* event) {
    pimpl->enter_event(*this, event);
    QAbstractButton::enterEvent(event);
}
void FilledButton::leaveEvent(QEvent* event) {
    pimpl->leave_event(*this, event);
    QAbstractButton::leaveEvent(event);
}

}
