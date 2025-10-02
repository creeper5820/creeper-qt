#include "sliders.hh"

#include "creeper-qt/utility/animation/animatable.hh"
#include "creeper-qt/utility/animation/state/pid.hh"
#include "creeper-qt/utility/animation/transition.hh"
#include "creeper-qt/utility/painter/helper.hh"

#include <qevent.h>
#include <qnamespace.h>
#include <qpainter.h>

/// TODO:
/// [ ] Adapt other directions
/// [ ] Add Disable status
/// [ ] Add Inset icon
/// [ ] Add Stops
/// [ ] Add Value indicator

using namespace creeper::slider::internal;

struct Slider::Impl {
public:
    explicit Impl(Slider& self) noexcept
        : self { self }
        , animatable { self } {

        // Transition For Handle Position
        {
            auto state = std::make_shared<PidState<double>>();

            state->config.kp      = 20.0;
            state->config.epsilon = 1e-4;

            position = make_transition(animatable, std::move(state));
        }
    }

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

    void set_color_scheme(const ColorScheme& scheme) {

        // Alpha 97 (约 38%): 用于禁用状态的文本、图标等前景元素 (Text/Icon).
        auto on_surface_disabled_foreground = scheme.on_surface;
        on_surface_disabled_foreground.setAlpha(97);

        // Alpha 31 (约 12%): 用于禁用状态的轨道、填充、容器等背景元素 (Container/Track).
        auto on_surface_disabled_container = scheme.on_surface;
        on_surface_disabled_container.setAlpha(31);

        // --- 启用 (Enabled) 状态映射 ---
        auto& enabled = color_specs.enabled;

        // 1. 值指示器 (气泡)
        enabled.value_indicator = scheme.inverse_surface;
        enabled.value_text      = scheme.inverse_on_surface;

        // 2. 停止点指示器 (Stop Indicators)
        enabled.stop_indicator_active   = scheme.primary;
        enabled.stop_indicator_inactive = scheme.secondary_container;

        // 3. 轨道 (Track)
        enabled.track_active   = scheme.primary;
        enabled.track_inactive = scheme.secondary_container;

        // 4. 拖动把手 (Handle)
        enabled.handle = scheme.primary;

        // --- 禁用 (Disabled) 状态映射 ---
        auto& disabled = color_specs.disabled;

        // 1. 值指示器 (气泡) - 气泡本身被禁用时，其背景和文字也应是禁用色
        disabled.value_indicator = on_surface_disabled_container;
        disabled.value_text      = on_surface_disabled_foreground;

        // 2. 停止点指示器
        disabled.stop_indicator_active   = on_surface_disabled_container;
        disabled.stop_indicator_inactive = on_surface_disabled_container;

        // 3. 轨道
        disabled.track_active   = on_surface_disabled_container;
        disabled.track_inactive = on_surface_disabled_container;

        // 4. 拖动把手
        disabled.handle = on_surface_disabled_container;
    }

    auto load_theme_manager(ThemeManager& manager) { manager.append_handler(&self, self); }

    auto set_progress(double progress, bool animatable = true) noexcept {
        this->progress = std::clamp(progress, 0.0, 1.0);
        if (animatable) {
            this->position->transition_to(progress);
        } else {
            this->position->snap_to(progress), self.update();
        }
    }

    auto get_progress() const noexcept { return progress; }

public:
    auto paint_event(QPaintEvent*) -> void {

        const auto& color = enabled ? color_specs.enabled : color_specs.disabled;

        // TODO: Develop some util to simplify those calculating
        const auto handle_spacing = double { 1.5 * measurements.handle_width };
        const auto common_radius  = double { 0.5 * measurements.handle_width };

        // Handle shape
        const auto handle_color     = color.handle;
        const auto handle_thickness = measurements.handle_width;
        const auto handle_length    = measurements.handle_height;
        const auto handle_radius    = 0.5 * handle_thickness;
        const auto handle_groove    = self.width() - 2 * handle_thickness;
        const auto handle_center    = is_horizontal()
               ? QPointF { handle_thickness + *position * handle_groove, 0.5 * self.height() }
               : QPointF { 0.5 * self.width(), handle_thickness + *position * handle_groove };

        const auto handle_thickness_real = pressed ? 0.5 * handle_thickness : handle_thickness;

        const auto handle_w = is_horizontal() ? handle_thickness_real : handle_length;
        const auto handle_h = is_horizontal() ? handle_length : handle_thickness_real;

        const auto handle_rectangle = QRectF {
            handle_center.x() - 0.5 * handle_w,
            handle_center.y() - 0.5 * handle_h,
            handle_w,
            handle_h,
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

        // Stop Indicator

        auto painter = QPainter { &self };

        util::PainterHelper { painter }
            .set_render_hint(QPainter::Antialiasing)

            // Track Part 1
            .rounded_rectangle(track_color_1, Qt::transparent, 0, track_1, track_shape,
                common_radius, common_radius, track_shape)

            // Track Part 2
            .rounded_rectangle(track_color_2, Qt::transparent, 0, track_2, common_radius,
                track_shape, track_shape, common_radius)

            // Stop Indicator
            // TODO:

            // Handle Shape
            .rounded_rectangle(
                handle_color, Qt::transparent, 0, handle_rectangle, handle_radius, handle_radius)

            // Done
            .done();
    }
    auto mouse_release_event(QMouseEvent* event) noexcept -> void {
        if (!enabled) return;

        pressed = false;
        update_progress(event->pos());
        emit self.signal_value_change_finished(progress);
    }
    auto mouse_press_event(QMouseEvent* event) noexcept -> void {
        if (!enabled) return;

        pressed = true;
        update_progress(event->pos());
        emit self.signal_value_change(progress);
    }
    auto mouse_move_event(QMouseEvent* event) noexcept -> void {
        if (!enabled) return;

        update_progress(event->pos());
        emit self.signal_value_change(progress);
    }

private:
    auto is_horizontal() const noexcept -> bool {
        return direction == Qt::RightArrow || direction == Qt::LeftArrow;
    }
    auto update_progress(const QPoint& point) noexcept -> void {
        const auto w = self.width();
        const auto h = self.height();
        const auto x = point.x();
        const auto y = point.y();

        auto spindle_len = int {};
        auto spindle_pos = int {};

        const auto thickness = measurements.handle_width;
        if (!is_horizontal()) {
            spindle_len = h - 2 * thickness;
            spindle_pos = y - 1 * thickness;
        } else {
            spindle_len = w - 2 * thickness;
            spindle_pos = x - 1 * thickness;
        }

        progress = static_cast<double>(spindle_pos) / spindle_len;
        progress = std::clamp(progress, 0., 1.);

        position->transition_to(progress);
    }

private:
    double progress = 0.0;
    uint steps      = 0;
    bool enabled    = true;
    bool pressed    = false;

    Qt::ArrowType direction = Qt::RightArrow;

    ColorSpecs color_specs    = ColorSpecs {};
    Measurements measurements = Measurements::Xs();

    Animatable animatable;
    std::unique_ptr<TransitionValue<PidState<double>>> position;

    Slider& self;
};
