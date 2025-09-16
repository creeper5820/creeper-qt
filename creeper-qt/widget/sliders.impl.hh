#include "creeper-qt/utility/painter/helper.hh"
#include "sliders.hh"
#include <qevent.h>
#include <qnamespace.h>
#include <qpainter.h>

using namespace creeper::slider::internal;

struct Slider::Impl {
public:
    explicit Impl(Slider& self) noexcept
        : self { self } { }

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
        auto painter = QPainter { &self };
        painter.setRenderHint(QPainter::Antialiasing);

        {
            painter.setPen(Qt::black);
            painter.setBrush(Qt::NoBrush);
            painter.drawRect(self.rect());
        }

        const auto& point_tl = self.geometry().bottomLeft();
        const auto& point_tr = self.geometry().bottomRight();
        const auto& point_bl = self.geometry().topLeft();
        const auto& point_br = self.geometry().topRight();

        const auto center_handle = is_horizontal()
            ? QPointF { progress * self.width(), static_cast<qreal>(self.height() / 2.) }
            : QPointF { static_cast<qreal>(self.width() / 2.), progress * self.height() };

        const auto center_l = QPointF { 0.0 * self.width(), 0.5 * self.height() };
        const auto center_r = QPointF { 1.0 * self.width(), 0.5 * self.height() };
        const auto center_t = QPointF { 0.5 * self.width(), 0.0 * self.height() };
        const auto center_b = QPointF { 0.5 * self.width(), 1.0 * self.height() };

        const auto h_half  = measurements.track_height / 2.;
        const auto track_1 = is_horizontal()
            ? QRectF { center_l + QPointF { 0, -h_half }, center_handle + QPointF { -5, +h_half } }
            : QRectF { center_t + QPointF { -h_half, 0 }, center_handle + QPointF { +h_half, -5 } };
        const auto track_2 = is_horizontal()
            ? QRectF { center_handle + QPointF { +5, -h_half }, center_r + QPointF { 0, +h_half } }
            : QRectF { center_handle + QPointF { -h_half, +5 }, center_b + QPointF { +h_half, 0 } };

        const auto track_color_1 = color_specs.enabled.track_active;
        const auto track_color_2 = color_specs.enabled.track_inactive;
        const auto track_shape   = measurements.track_shape;

        util::PainterHelper { painter }
            .rounded_rectangle(
                track_color_1, Qt::transparent, 0, track_1, track_shape, 2, 2, track_shape)
            .rounded_rectangle(
                track_color_2, Qt::transparent, 0, track_2, 2, track_shape, track_shape, 2)
            .done();
    }
    auto mouse_release_event(QMouseEvent* event) noexcept -> void {
        if (changed) {
            changed = false;
            emit self.signal_value_change_finished(progress);
        }
    }
    auto mouse_press_event(QMouseEvent* event) noexcept -> void {
        // TODO: 判断值是否改变
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
        auto spindle_length   = int {};
        auto spindle_position = int {};
        if (!is_horizontal()) {
            spindle_length   = self.height();
            spindle_position = position.y();
        } else {
            spindle_length   = self.width();
            spindle_position = position.x();
        }
        progress = static_cast<double>(spindle_position) / spindle_length;
        progress = std::clamp(progress, 0., 1.);

        self.update();
    }

private:
    double progress = 0.0;
    int steps       = 0;
    bool changed    = false;

    Qt::ArrowType direction = Qt::RightArrow;

    ColorSpecs color_specs;
    Measurements measurements = Measurements::Xs();

    Slider& self;

    static constexpr auto handle_spacing = int { 8 };
};
