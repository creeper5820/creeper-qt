#pragma once
#include "creeper-qt/utility/animation/animatable.hh"
#include "creeper-qt/utility/animation/math.hh"
#include "creeper-qt/utility/animation/state/pid.hh"
#include "creeper-qt/utility/animation/state/spring.hh"
#include "creeper-qt/utility/animation/state/tween.hh"
#include "creeper-qt/utility/animation/transition.hh"
#include "creeper-qt/utility/painter/container.hh"
#include "creeper-qt/utility/painter/shape.hh"

#include <qevent.h>
#include <qpainter.h>

#include <algorithm>
#include <cmath>

#include "switch.hh"

using namespace creeper;

struct Switch::Impl {

    bool checked  = false;
    bool disabled = false;

    Measurements measurements;
    Colors colors;

    static constexpr double kPositionUnchecked = 0.0;
    static constexpr double kPositionChecked   = 1.0;

    Animatable animatable;

    std::unique_ptr<TransitionValue<PidState<Eigen::Vector4d>>> track;
    std::unique_ptr<TransitionValue<PidState<Eigen::Vector4d>>> handle;
    std::unique_ptr<TransitionValue<PidState<Eigen::Vector4d>>> outline;

    std::unique_ptr<TransitionValue<SpringState<double>>> position;

    // 状态层各自独立：hover / focus / press 的不透明度进度，press_expansion 驱动 handle 放大
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
        update_switch_ui(self);
    }

    void set_checked(Switch& self, bool on) {
        if (checked == on) return;

        checked = on;
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
        if (disabled) return;

        press->get_state().config.duration = Defaults::kFadeInDuration;
        press->transition_to(1.0);

        press_expansion->snap_to(1.0);
        self.update();
    }

    void mouse_release_event(Switch& self, const QMouseEvent& event) {
        press->get_state().config.duration = Defaults::kFadeOutDuration;
        press->transition_to(0.0);

        press_expansion->transition_to(0.0);
        self.update();
    }

    void paint_event(Switch& self, const QPaintEvent& event) const {
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

        hover_color.setAlphaF(Defaults::kHoverStateLayerOpacity * double { *hover });
        focus_color.setAlphaF(Defaults::kFocusStateLayerOpacity * double { *focus });
        press_color.setAlphaF(Defaults::kPressedStateLayerOpacity * double { *press });

        const auto size_track  = qt::size { track_width, track_height };
        const auto size_handle = qt::size { handle_radius * 2, handle_radius * 2 };
        const auto size_layer  = qt::size { layer_radius * 2, layer_radius * 2 };

        const auto origin_layer = handle_point - QPointF { layer_radius, layer_radius };

        using namespace painter;
        using namespace painter::common::pro;

        auto paint = Paint::Box {
            BoxImpl { self.size(), Qt::AlignCenter },
            Paint::Surface {
              SurfaceImpl { size_track },
              Paint::RoundedRectangle {
                Size { size_track },
                Fill { from_vector4(*track) },
                Outline { from_vector4(*outline), outline_width },
                Radiuses { track_radius },
              },
              Paint::Box {
                BoxImpl { size_layer, Qt::AlignCenter, origin_layer },
                Paint::Ellipse {
                  Size { size_handle },
                  Fill { from_vector4(*handle) },
                },
                Paint::Ellipse {
                  Size { size_layer },
                  Fill { hover_color },
                },
                Paint::Ellipse {
                  Size { size_layer },
                  Fill { focus_color },
                },
                Paint::Ellipse {
                  Size { size_layer },
                  Fill { press_color },
                },
              },
            },
        };

        auto painter = QPainter { &self };
        paint(painter);
    }

private:
    auto update_switch_ui(Switch& self) -> void {

        // 添加 Switch 轨道颜色动画
        const auto track_target = disabled
            ? (checked ? colors.disabled_checked_track_color
                       : colors.disabled_unchecked_track_color)
            : (checked ? colors.checked_track_color : colors.unchecked_track_color);
        track->transition_to(from_color(track_target));

        // 添加 Switch 指示器颜色动画
        const auto handle_target = disabled
            ? (checked ? colors.disabled_checked_thumb_color
                       : colors.disabled_unchecked_thumb_color)
            : (checked ? colors.checked_thumb_color : colors.unchecked_thumb_color);
        handle->transition_to(from_color(handle_target));

        // 添加 Switch 外边框颜色动画
        const auto outline_target = disabled
            ? (checked ? colors.disabled_checked_border_color
                       : colors.disabled_unchecked_border_color)
            : (checked ? colors.checked_border_color : colors.unchecked_border_color);
        outline->transition_to(from_color(outline_target));

        // 添加 Switch 运动动画
        const auto position_target = checked ? kPositionChecked : kPositionUnchecked;
        position->transition_to(position_target);

        self.update();
    }
};
