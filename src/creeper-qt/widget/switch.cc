#include "switch.hh"

#include "creeper-qt/details/drawable/core.hh"
#include "creeper-qt/details/drawable/layout/box.hh"
#include "creeper-qt/details/drawable/layout/surface.hh"
#include "creeper-qt/details/drawable/shape/ellipse.hh"
#include "creeper-qt/details/drawable/shape/rect.hh"
#include "creeper-qt/utility/animation/animatable.hh"
#include "creeper-qt/utility/animation/math.hh"
#include "creeper-qt/utility/animation/state/pid.hh"
#include "creeper-qt/utility/animation/state/spring.hh"
#include "creeper-qt/utility/animation/state/tween.hh"
#include "creeper-qt/utility/animation/transition.hh"

#include <qevent.h>
#include <qpainter.h>

#include <algorithm>
#include <cmath>

using namespace creeper;

struct Switch::Impl {
    static constexpr double kPositionUnchecked = 0.0;
    static constexpr double kPositionChecked   = 1.0;

    bool checked  = false;
    bool disabled = false;

    Measurements measurements;
    Colors colors;

    Animatable animatable;

    std::unique_ptr<TransitionValue<PidState<Eigen::Vector4d>>> track;
    std::unique_ptr<TransitionValue<PidState<Eigen::Vector4d>>> handle;
    std::unique_ptr<TransitionValue<PidState<Eigen::Vector4d>>> outline;

    std::unique_ptr<TransitionValue<SpringState<double>>> position;

    std::unique_ptr<TransitionValue<TweenState<double>>> hover;
    std::unique_ptr<TransitionValue<TweenState<double>>> focus;
    std::unique_ptr<TransitionValue<TweenState<double>>> press;
    std::unique_ptr<TransitionValue<SpringState<double>>> press_expansion;

    explicit Impl(Switch& self) noexcept
        : animatable(self) {

        const auto stiffness = Defaults::kSpringFastSpatialStiffness;
        const auto damping   = 2 * Defaults::kSpringFastSpatialDamping * std::sqrt(stiffness);

        const auto proportional_gain = damping / 2;
        {
            const auto make_state = [proportional_gain] {
                auto state = std::make_shared<PidState<Eigen::Vector4d>>();

                state->config.kp = proportional_gain;
                state->config.ki = 0.0;
                state->config.kd = 0.0;

                return state;
            };
            track   = make_transition(animatable, make_state());
            handle  = make_transition(animatable, make_state());
            outline = make_transition(animatable, make_state());
        }
        {
            const auto make_state = [stiffness, damping] {
                auto state = std::make_shared<SpringState<double>>();

                state->config.k       = stiffness;
                state->config.d       = damping;
                state->config.epsilon = 1e-4;

                return state;
            };
            position        = make_transition(animatable, make_state());
            press_expansion = make_transition(animatable, make_state());
        }
        {
            const auto make_state = [] {
                auto state = std::make_shared<TweenState<double>>();

                state->config.duration = Defaults::kStateLayerDefaultDuration;
                state->config.easing   = Defaults::kEasingLinearCubicBezier;

                return state;
            };
            hover = make_transition(animatable, make_state());
            focus = make_transition(animatable, make_state());
            press = make_transition(animatable, make_state());
        }

        self.setCheckable(true);
        self.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        set_measurements(self, measurements);
    }

    void load_colorscheme(Switch& self, const ColorScheme& scheme) {
        Defaults::mapColors(scheme, colors);
        update_switch_ui(self);
    }

    void set_measurements(Switch& self, const Measurements& value) {
        measurements = value;
        self.setMinimumSize(size_hint());
        self.updateGeometry();
    }

    auto size_hint() const -> QSize {
        const auto width =
            measurements.track_width + (measurements.state_layer_size - measurements.track_height);
        const auto height = std::max(measurements.state_layer_size, measurements.touch_target_size);

        return QSize { width.px(), height.px() };
    }

    void set_colors(Switch& self, const Colors& value) {
        colors = value;
        update_switch_ui(self);
    }

    void set_disabled(Switch& self, bool on) {
        if (disabled == on) return;

        disabled = on;
        self.setEnabled(!on);

        if (on) {
            hover->transition_to(0.0);
            focus->transition_to(0.0);
            press->transition_to(0.0);
            press_expansion->transition_to(0.0);
        }

        update_switch_ui(self);
    }

    void set_checked(Switch& self, bool on) {
        if (checked == on) return;

        checked = on;
        self.QAbstractButton::setChecked(on);
        update_switch_ui(self);
    }

    void enter_event(Switch& self, const QEvent& event) {
        if (disabled) return;

        self.setCursor(Qt::PointingHandCursor);
        hover->transition_to(1.0);
        self.update();
    }

    void leave_event(Switch& self, const QEvent& event) {
        hover->transition_to(0.0);
        self.update();
    }

    void focus_in_event(Switch& self, const QFocusEvent& event) {
        const auto keyboard =
            (event.reason() == Qt::TabFocusReason) || (event.reason() == Qt::BacktabFocusReason);

        focus->get_state().config.duration =
            keyboard ? Defaults::kStateLayerFocusInDuration : Defaults::kStateLayerDefaultDuration;
        focus->transition_to(keyboard ? 1.0 : 0.0);
        self.update();
    }

    void focus_out_event(Switch& self, const QFocusEvent& event) {
        focus->get_state().config.duration = Defaults::kStateLayerDefaultDuration;
        focus->transition_to(0.0);
        self.update();
    }

    void mouse_press_event(Switch& self, const QMouseEvent& event) {
        if (event.button() != Qt::LeftButton) return;
        if (disabled) return;

        press->get_state().config.duration = Defaults::kFadeInDuration;
        press->transition_to(1.0);

        press_expansion->snap_to(1.0);
        self.update();
    }

    void mouse_release_event(Switch& self, const QMouseEvent& event) {
        if (event.button() != Qt::LeftButton) return;

        press->get_state().config.duration = Defaults::kFadeOutDuration;
        press->transition_to(0.0);

        press_expansion->transition_to(0.0);
        self.update();
    }

    void paint_event(Switch& self, const QPaintEvent& event) const {
        using namespace drawable;

        const auto track_width   = measurements.track_width.value;
        const auto track_height  = measurements.track_height.value;
        const auto outline_width = measurements.track_outline_width.value;
        const auto track_radius  = track_height / 2;
        const auto layer_radius  = measurements.state_layer_size.value / 2;

        // handle 直径在未选中与选中之间插值，按下时再放大到 pressed_thumb_diameter
        const auto handle_radius_unchecked = measurements.unchecked_thumb_diameter.value / 2;
        const auto handle_radius_checked   = measurements.thumb_diameter.value / 2;
        const auto handle_radius_pressed   = measurements.pressed_thumb_diameter.value / 2;

        const auto check_progress = double { *position };
        const auto press_progress = std::clamp(double { *press_expansion }, 0.0, 1.0);

        const auto handle_radius_base =
            std::lerp(handle_radius_unchecked, handle_radius_checked, check_progress);
        const auto handle_radius =
            std::lerp(handle_radius_base, handle_radius_pressed, press_progress);

        // handle 中心在 track 坐标系内，从左 / 右各内缩 track_radius 的位置之间插值
        const auto handle_point_begin = QPointF { track_radius, track_radius };
        const auto handle_point_final = QPointF { track_width - track_radius, track_radius };

        const auto handle_point =
            handle_point_begin + check_progress * (handle_point_final - handle_point_begin);

        auto hover_color = colors.state_layer_color;
        auto focus_color = colors.state_layer_color;
        auto press_color = colors.state_layer_color;

        hover_color.setAlphaF(Defaults::kHoverStateLayerOpacity * *hover);
        focus_color.setAlphaF(Defaults::kFocusStateLayerOpacity * *focus);
        press_color.setAlphaF(Defaults::kPressedStateLayerOpacity * *press);

        const auto size_track  = QSizeF { track_width, track_height };
        const auto size_handle = QSizeF { handle_radius * 2, handle_radius * 2 };
        const auto size_layer  = QSizeF { layer_radius * 2, layer_radius * 2 };

        const auto origin_layer = handle_point - QPointF { layer_radius, layer_radius };

        auto painter = QPainter { &self };
        DrawHere {
            painter,
            Box {
              {
                .size  = QSizeF { self.size() },
                .align = Qt::AlignCenter,
              },
              Surface {
                {
                  .size   = size_track,
                  .origin = { },
                },
                Rect {
                  .size  = size_track,
                  .color = from_vector4(*track),

                  .outline_color = from_vector4(*outline),
                  .outline_width = outline_width,

                  .radius_tl = track_radius,
                  .radius_tr = track_radius,
                  .radius_br = track_radius,
                  .radius_bl = track_radius,
                },
                Surface {
                  {
                    .size   = size_layer,
                    .origin = origin_layer,
                  },
                  Box {
                    {
                      .size  = size_layer,
                      .align = Qt::AlignCenter,
                    },
                    Ellipse {
                      .size  = size_handle,
                      .color = from_vector4(*handle),
                    },
                    Ellipse {
                      .size  = size_layer,
                      .color = hover_color,
                    },
                    Ellipse {
                      .size  = size_layer,
                      .color = focus_color,
                    },
                    Ellipse {
                      .size  = size_layer,
                      .color = press_color,
                    },
                  },
                },
              },
            },
        };
    }

private:
    auto update_switch_ui(Switch& self) -> void {

        // 轨道颜色动画
        const auto track_target = disabled
            ? (checked ? colors.disabled_checked_track_color
                       : colors.disabled_unchecked_track_color)
            : (checked ? colors.checked_track_color : colors.unchecked_track_color);
        track->transition_to(from_color(track_target));

        // 指示器颜色动画
        const auto handle_target = disabled
            ? (checked ? colors.disabled_checked_thumb_color
                       : colors.disabled_unchecked_thumb_color)
            : (checked ? colors.checked_thumb_color : colors.unchecked_thumb_color);
        handle->transition_to(from_color(handle_target));

        // 外边框颜色动画
        const auto outline_target = disabled
            ? (checked ? colors.disabled_checked_border_color
                       : colors.disabled_unchecked_border_color)
            : (checked ? colors.checked_border_color : colors.unchecked_border_color);
        outline->transition_to(from_color(outline_target));

        // 运动动画
        const auto position_target = checked ? kPositionChecked : kPositionUnchecked;
        position->transition_to(position_target);

        self.update();
    }
};

Switch::Switch()
    : pimpl(std::make_unique<Impl>(*this)) { }

Switch::~Switch() = default;

auto Switch::loadColorScheme(const ColorScheme& scheme) -> void {
    pimpl->load_colorscheme(*this, scheme);
}

auto Switch::bindThemeManager(ThemeManager& manager) -> void {
    manager.appendHandler(
        this, [this](const ThemeManager& manager) { loadColorScheme(manager.colorScheme()); });
}

auto Switch::setMeasurements(const Measurements& measurements) -> void {
    pimpl->set_measurements(*this, measurements);
}

auto Switch::setColors(const Colors& colors) -> void {
    pimpl->set_colors(*this, colors);
}

auto Switch::sizeHint() const -> QSize {
    return pimpl->size_hint();
}

auto Switch::setDisabled(bool on) -> void {
    pimpl->set_disabled(*this, on);
}
auto Switch::disabled() const -> bool {
    return pimpl->disabled;
}

auto Switch::setChecked(bool on) -> void {
    pimpl->set_checked(*this, on);
}
auto Switch::checked() const -> bool {
    return pimpl->checked;
}

auto Switch::enterEvent(qt::EnterEvent* event) -> void {
    pimpl->enter_event(*this, *event);
    QAbstractButton::enterEvent(event);
}

auto Switch::leaveEvent(QEvent* event) -> void {
    pimpl->leave_event(*this, *event);
    QAbstractButton::leaveEvent(event);
}

auto Switch::focusInEvent(QFocusEvent* event) -> void {
    pimpl->focus_in_event(*this, *event);
    QAbstractButton::focusInEvent(event);
}

auto Switch::focusOutEvent(QFocusEvent* event) -> void {
    pimpl->focus_out_event(*this, *event);
    QAbstractButton::focusOutEvent(event);
}

auto Switch::mousePressEvent(QMouseEvent* event) -> void {
    pimpl->mouse_press_event(*this, *event);
    QAbstractButton::mousePressEvent(event);
}

auto Switch::mouseReleaseEvent(QMouseEvent* event) -> void {
    pimpl->mouse_release_event(*this, *event);
    QAbstractButton::mouseReleaseEvent(event);
}

auto Switch::nextCheckState() -> void { }

auto Switch::paintEvent(QPaintEvent* event) -> void {
    pimpl->paint_event(*this, *event);
}
