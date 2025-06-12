#include "filled-button.hh"

#include <qevent.h>
#include <qpainterpath.h>
#include <qtimer.h>

#include "utility/painter/helper.hh"
using creeper::util::PainterHelper;

#include "utility/animation/core.hh"
#include "utility/animation/math.hh"
#include "utility/animation/water-ripple.hh"
using namespace creeper::util::animation;

namespace creeper::filled_button::internal {

struct FilledButton::Impl {
public:
    AnimationCore animation_core;
    bool enable_water_ripple = true;
    double water_ripple_step = 3.;

    double radius = 0;
    QColor text_color = Qt::black;
    QColor background = Qt::white;

    QColor border_color = Qt::red;
    double border_width = 0;

    QColor water_color = Qt::black;

    double hover_opacity = 0;
    bool is_mouse_hover = false;

    explicit Impl(QPushButton& self)
        : animation_core(self, 90) { }

    void paint_event(QPushButton& self, QPaintEvent* event) {
        auto painter = QPainter { &self };

        PainterHelper { painter }
            .set_render_hint(QPainter::RenderHint::Antialiasing)

            // Background
            .set_opacity(1)
            .rounded_rectangle(background, border_color, border_width, self.rect(), radius, radius)

            // Hover Animation
            .set_opacity(
                hover_opacity = update_pid_using_target(hover_opacity, get_hover_opacity_target()))
            .rounded_rectangle(background, Qt::transparent, 0, self.rect(), radius, radius)
            .done();

        animation_core.paint_event(*event);
    }

    void mouse_release_event(QPushButton& self, QMouseEvent* event) {
        if (enable_water_ripple) {
            const auto path = make_rounded_rectangle_path(self.rect(), radius);
            const auto animation = WaterRipple { self, path, event->pos(), water_color,
                std::max<double>(self.width(), self.height()), water_ripple_step, 0.5 };
            animation_core.append_animation(std::make_unique<WaterRipple>(std::move(animation)));
        }
    }

    void enter_event(QPushButton& self, QEvent* event) { is_mouse_hover = true; }

    void leave_event(QPushButton& self, QEvent* event) { is_mouse_hover = false; }

private:
    constexpr static inline double mouse_hover_opacity = 0.5;
    constexpr static inline double mouse_leave_opacity = 0.0;
    double get_hover_opacity_target() {
        return is_mouse_hover ? mouse_hover_opacity : mouse_leave_opacity;
    }

    static QPainterPath make_rounded_rectangle_path(const QRectF& rect, double radius) {
        auto path = QPainterPath {};
        path.addRoundedRect(rect, radius, radius);
        return path;
    }
};

FilledButton::FilledButton()
    : pimpl(std::make_unique<Impl>(*this)) { }

FilledButton::~FilledButton() = default;

// 属性设置接口实现

void FilledButton::set_radius(double radius) { pimpl->radius = radius; }

void FilledButton::set_border_width(double border) { pimpl->border_width = border; }

void FilledButton::set_border_color(const QColor& color) { pimpl->border_color = color; }

void FilledButton::set_water_color(const QColor& color) { pimpl->water_color = color; }

void FilledButton::set_text_color(const QColor& color) { pimpl->text_color = color; }

void FilledButton::set_background(const QColor& color) { pimpl->background = color; }

void FilledButton::set_water_ripple_status(bool enable) { pimpl->enable_water_ripple = enable; }

void FilledButton::set_water_ripple_step(double step) { pimpl->water_ripple_step = step; }

// Qt 接口重载

void FilledButton::mouseReleaseEvent(QMouseEvent* event) {
    pimpl->mouse_release_event(*this, event);
    QPushButton::mouseReleaseEvent(event);
}
void FilledButton::paintEvent(QPaintEvent* event) {
    pimpl->paint_event(*this, event);
    /* Disable QPushButton::paintEvent */;
}
void FilledButton::enterEvent(QEvent* event) {
    pimpl->enter_event(*this, event);
    QPushButton::enterEvent(event);
}
void FilledButton::leaveEvent(QEvent* event) {
    pimpl->leave_event(*this, event);
    QPushButton::leaveEvent(event);
}

}
