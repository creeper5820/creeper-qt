#include "sliders.hh"

#include "creeper-qt/utility/animation/animatable.hh"
#include "creeper-qt/utility/painter/helper.hh"

#include <qevent.h>
#include <qnamespace.h>
#include <qpainter.h>

using namespace creeper::slider::internal;

struct Slider::Impl {
public:
    explicit Impl(Slider& self) noexcept
        : self { self }
        , animatable { self } { }

    auto set_direction(Qt::ArrowType direction) noexcept -> void {
        this->direction = direction;
        self.update();
    }
    auto set_color_specs(const ColorSpecs& color_specs) noexcept -> void {
        this->color_specs = color_specs;
        self.update();
    }
    auto set_measurements(const Measurements& measurements) noexcept -> void {
        this->measurements = measurements;
        self.update();
    }

public:
    auto paint_event(QPaintEvent*) -> void {

        const auto& color = enabled ? color_specs.enabled : color_specs.disabled;

        auto painter = QPainter { &self };
        painter.setRenderHint(QPainter::Antialiasing);

        // TODO: Develop some util to simplify those calculating
        constexpr auto handle_spacing = int { 4 };

        // Points of 4 corner
        const auto& point_tl = self.geometry().bottomLeft();
        const auto& point_tr = self.geometry().bottomRight();
        const auto& point_bl = self.geometry().topLeft();
        const auto& point_br = self.geometry().topRight();

        // Handle shape
        const auto handle_color     = color.handle;
        const auto handle_thickness = measurements.handle_width;
        const auto handle_length    = measurements.handle_height;
        const auto handle_radius    = 0.5 * handle_thickness;
        const auto handle_groove    = self.width() - 2 * handle_thickness;
        const auto handle_center    = is_horizontal()
               ? QPointF { handle_thickness + progress * handle_groove, 0.5 * self.height() }
               : QPointF { 0.5 * self.width(), handle_thickness + progress * handle_groove };

        const auto handle_thickness_real = pressed ? 0.5 * handle_thickness : handle_thickness;

        const auto handle_w = is_horizontal() ? handle_thickness_real : handle_length;
        const auto handle_h = is_horizontal() ? handle_length : handle_thickness_real;

        const auto handle_rectangle = QRectF {
            handle_center.x() - handle_w / 2.0,
            handle_center.y() - handle_h / 2.0,
            static_cast<qreal>(handle_w),
            static_cast<qreal>(handle_h),
        };

        // Outline center of 4 sides
        const auto center_l = QPointF { 0.0 * self.width(), 0.5 * self.height() };
        const auto center_r = QPointF { 1.0 * self.width(), 0.5 * self.height() };
        const auto center_t = QPointF { 0.5 * self.width(), 0.0 * self.height() };
        const auto center_b = QPointF { 0.5 * self.width(), 1.0 * self.height() };

        // Track shape
        const auto half_h  = measurements.track_height / 2.;
        const auto track_1 = is_horizontal()
            ? QRectF { center_l + QPointF { 0, -half_h },
                  handle_center + QPointF { -handle_spacing, +half_h } }
            : QRectF { center_t + QPointF { -half_h, 0 },
                  handle_center + QPointF { +half_h, -handle_spacing } };
        const auto track_2 = is_horizontal()
            ? QRectF { handle_center + QPointF { +handle_spacing, -half_h },
                  center_r + QPointF { 0, +half_h } }
            : QRectF { handle_center + QPointF { -half_h, +handle_spacing },
                  center_b + QPointF { +half_h, 0 } };

        const auto track_color_1 = color.track_active;
        const auto track_color_2 = color.track_inactive;
        const auto track_shape   = measurements.track_shape;

        util::PainterHelper { painter }

            // Track Part 1
            .rounded_rectangle(
                track_color_1, Qt::transparent, 0, track_1, track_shape, 2, 2, track_shape)

            // Track Part 2
            .rounded_rectangle(
                track_color_2, Qt::transparent, 0, track_2, 2, track_shape, track_shape, 2)

            // Stop Indicator
            // TODO:

            // Handle Shape
            .rounded_rectangle(
                handle_color, Qt::transparent, 0, handle_rectangle, handle_radius, handle_radius)

            // Done
            .done();
    }
    auto mouse_release_event(QMouseEvent* event) noexcept -> void {
        pressed = false;
        update_progress(event->pos());
        emit self.signal_value_change_finished(progress);
    }
    auto mouse_press_event(QMouseEvent* event) noexcept -> void {
        pressed = true;
        update_progress(event->pos());
        emit self.signal_value_change(progress);
    }
    auto mouse_move_event(QMouseEvent* event) noexcept -> void {
        update_progress(event->pos());
        emit self.signal_value_change(progress);
    }

private:
    auto is_horizontal() const noexcept -> bool {
        return direction == Qt::RightArrow || direction == Qt::LeftArrow;
    }
    auto update_progress(const QPoint& position) noexcept -> void {
        const auto w = self.width();
        const auto h = self.height();
        const auto x = position.x();
        const auto y = position.y();

        auto handle_thickness = measurements.handle_width;
        auto spindle_len      = int {};
        auto spindle_pos      = int {};

        if (!is_horizontal()) {
            spindle_len = h - 2 * handle_thickness;
            spindle_pos = y - 1 * handle_thickness;
        } else {
            spindle_len = w - 2 * handle_thickness;
            spindle_pos = x - 1 * handle_thickness;
        }

        progress = static_cast<double>(spindle_pos) / spindle_len;
        progress = std::clamp(progress, 0., 1.);

        self.update();
    }

private:
    double progress = 0.0;
    int steps       = 0;
    bool changed    = false;
    bool enabled    = true;
    bool pressed    = false;

    Qt::ArrowType direction = Qt::RightArrow;

    ColorSpecs color_specs    = ColorSpecs {};
    Measurements measurements = Measurements::Xs();

    Slider& self;
    Animatable animatable;
};
