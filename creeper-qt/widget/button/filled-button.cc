#include "filled-button.hh"

#include <qevent.h>
#include <qpainterpath.h>
#include <qtimer.h>

#include "utility/painter/helper.hh"
using creeper::util::PainterHelper;

#include "utility/animation/math.hh"
using creeper::util::animation::update_pid_using_target;

namespace creeper::filled_button::internal {

struct FilledButton::Impl {
public:
    QTimer animation_scheduler;

    double radius = 0;
    QColor text_color = Qt::black;
    QColor background = Qt::white;

    QColor border_color = Qt::red;
    double border_width = 0;

    QColor water_color = Qt::black;

    double hover_opacity = 0;
    bool is_mouse_hover = false;

    struct WaterRipple {
        QPointF origin;
        double distance = 0;
    };
    std::vector<WaterRipple> animations;

    explicit Impl(QPushButton& self) {
        animation_scheduler.setInterval(1'000 / 60);
        QObject::connect(&animation_scheduler, &QTimer::timeout, [&self] { self.update(); });
    }

    void paint_event(QPushButton& self, QPaintEvent* event) {
        auto painter = QPainter { &self };

        auto clip_path = QPainterPath {};
        clip_path.addRoundedRect(self.rect(), radius, radius);

        PainterHelper { painter }
            .set_render_hint(QPainter::RenderHint::Antialiasing)

            // Background
            .set_opacity(1)
            .rounded_rectangle(background, border_color, border_width, self.rect(), radius, radius)

            // Hover Animation
            .set_opacity(
                hover_opacity = update_pid_using_target(hover_opacity, get_hover_opacity_target()))
            .rounded_rectangle(background, Qt::transparent, 0, self.rect(), radius, radius)

            // Water Ripple Animation
            .apply([&, this](QPainter& painter) {
                const auto update_context = [&, this](WaterRipple& context) {
                    const double max_distance = std::max(self.width(), self.height());
                    const auto& [origin, distance] = context;
                    PainterHelper { painter }
                        .set_clip_path(clip_path)
                        .set_opacity(0.3 * (1. - distance / max_distance))
                        .ellipse(water_color, Qt::transparent, 0, origin, distance, distance);
                    return (context.distance += 5.) > max_distance;
                };
                const auto [first, last] = std::ranges::remove_if(animations, update_context);
                animations.erase(first, last);

                if (animations.empty()) animation_scheduler.stop();
            })
            .done();
    }

    void mouse_release_event(QPushButton& self, QMouseEvent* event) {
        animations.emplace_back(event->pos());
        if (!animation_scheduler.isActive()) animation_scheduler.start();
    }

    void enter_event(QEvent* event) { }

    void leave_event(QEvent* event) { }

private:
    constexpr static inline double mouse_hover_opacity = 0.5;
    constexpr static inline double mouse_leave_opacity = 0.0;
    double get_hover_opacity_target() {
        return is_mouse_hover ? mouse_hover_opacity : mouse_leave_opacity;
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
