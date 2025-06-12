#include "filled-button.hh"
#include <iostream>

#include <qevent.h>
#include <qpainterpath.h>
#include <qtimer.h>

#include "utility/painter/helper.hh"
using creeper::util::PainterHelper;

#include "utility/animation/math.hh"
using creeper::util::animation::update_pid_using_target;

#include "utility/animation/core.hh"
using creeper::util::animation::AnimationCore;

#include "utility/animation/water-ripple.hh"
using creeper::util::animation::WaterRipple;

namespace creeper::filled_button::internal {

struct FilledButton::Impl {
public:
    AnimationCore animation_core;

    double radius = 0;
    QColor text_color = Qt::black;
    QColor background = Qt::white;

    QColor border_color = Qt::red;
    double border_width = 0;

    QColor water_color = Qt::black;

    double hover_opacity = 0;
    bool is_mouse_hover = false;

    explicit Impl(QPushButton& self)
        : animation_core(self, 10) { }

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
        const auto path = make_rounded_rectangle_path(self.rect(), radius);
        const auto animation = WaterRipple { self, path, event->pos(), water_color,
            std::max<double>(self.width(), self.height()), 5, 0.5 };
        animation_core.append_animation(std::make_unique<WaterRipple>(std::move(animation)));
    }

    void enter_event(QEvent* event) { is_mouse_hover = true; }

    void leave_event(QEvent* event) { is_mouse_hover = false; }

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

void FilledButton::set_water_ripple_status(bool enable) { }

void FilledButton::set_water_ripple_step(double step) { }

// Qt 接口重载

void FilledButton::mouseReleaseEvent(QMouseEvent* event) {
    pimpl->mouse_release_event(*this, event);
    QPushButton::mouseReleaseEvent(event);
}

void FilledButton::paintEvent(QPaintEvent* event) { pimpl->paint_event(*this, event); }

void FilledButton::enterEvent(QEvent* event) { QPushButton::enterEvent(event); }

void FilledButton::leaveEvent(QEvent* event) { QPushButton::leaveEvent(event); }

}
