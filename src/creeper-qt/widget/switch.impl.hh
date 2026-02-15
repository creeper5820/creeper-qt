#pragma once
#include "switch.hh"

#include <qpainter.h>

#include "creeper-qt/utility/animation/animatable.hh"
#include "creeper-qt/utility/animation/state/pid.hh"
#include "creeper-qt/utility/animation/state/spring.hh"
#include "creeper-qt/utility/animation/transition.hh"
#include "creeper-qt/utility/painter/helper.hh"

using namespace creeper::_switch::internal;

struct Switch::Impl {

    bool checked  = false;
    bool disabled = false;
    bool hovered  = false;

    QColor track_unchecked;
    QColor track_checked;
    QColor track_unchecked_disabled;
    QColor track_checked_disabled;

    QColor handle_unchecked;
    QColor handle_checked;
    QColor handle_unchecked_disabled;
    QColor handle_checked_disabled;

    QColor outline_unchecked;
    QColor outline_checked;
    QColor outline_unchecked_disabled;
    QColor outline_checked_disabled;

    QColor hover_unchecked;
    QColor hover_checked;

    static constexpr double position_unchecked = 0.0;
    static constexpr double position_checked   = 1.0;

    Animatable animatable;

    std::unique_ptr<TransitionValue<PidState<Eigen::Vector4d>>> track;
    std::unique_ptr<TransitionValue<PidState<Eigen::Vector4d>>> handle;
    std::unique_ptr<TransitionValue<PidState<Eigen::Vector4d>>> outline;

    std::unique_ptr<TransitionValue<SpringState<double>>> position;

    explicit Impl(Switch& self) noexcept
        : animatable(self) {

        // @TODO: 适配进 MotionScheme
        constexpr double kp = 15.0, ki = 0.0, kd = 0.0, hz = 90;
        constexpr double k = 400, d = 22;
        {
            constexpr auto make_state = [] {
                auto state = std::make_shared<PidState<Eigen::Vector4d>>();

                state->config.kp = kp;
                state->config.ki = ki;
                state->config.kd = kd;

                return state;
            };
            track   = make_transition(animatable, make_state());
            handle  = make_transition(animatable, make_state());
            outline = make_transition(animatable, make_state());
        }
        {
            auto state = std::make_shared<SpringState<double>>();

            state->config.k = k;
            state->config.d = d;

            position = make_transition(animatable, std::move(state));
        }

        QObject::connect(&self, &Switch::clicked, [this, &self] { set_checked(self, !checked); });
    }

    void set_color_scheme(Switch& self, const ColorScheme& scheme) {
        track_unchecked          = scheme.surface_variant;
        track_checked            = scheme.primary;
        track_unchecked_disabled = scheme.surface_variant;
        track_checked_disabled   = scheme.on_surface;

        handle_unchecked          = scheme.outline;
        handle_checked            = scheme.on_primary;
        handle_unchecked_disabled = scheme.on_surface;
        handle_checked_disabled   = scheme.surface;

        outline_unchecked          = scheme.outline;
        outline_checked            = scheme.primary;
        outline_unchecked_disabled = scheme.on_surface;
        outline_checked_disabled   = Qt::transparent;

        hover_checked   = scheme.primary;
        hover_unchecked = scheme.on_surface;

        constexpr auto disabled_track_opacity = 0.12;
        track_unchecked_disabled.setAlphaF(disabled_track_opacity);
        track_checked_disabled.setAlphaF(disabled_track_opacity);
        outline_unchecked_disabled.setAlphaF(disabled_track_opacity);

        constexpr auto disabled_handle_opacity = 0.38;
        handle_unchecked_disabled.setAlphaF(disabled_handle_opacity);
        handle_checked_disabled.setAlphaF(disabled_handle_opacity);

        constexpr auto hover_opacity = 0.08;
        hover_checked.setAlphaF(hover_opacity);
        hover_unchecked.setAlphaF(hover_opacity);

        update_switch_ui(self, checked, disabled);
    }

    void set_disabled(Switch& self, bool on) {
        if (disabled == on) return;

        disabled = on;
        update_switch_ui(self, checked, on);
    }

    void set_checked(Switch& self, bool on) {
        if (disabled || checked == on) return;

        checked = on;
        update_switch_ui(self, on);
    }

    void enter_event(Switch& self, const QEvent& event) {
        if (!disabled) self.setCursor(Qt::PointingHandCursor);
        hovered = true;
    }

    void leave_event(Switch& self, const QEvent& event) { hovered = false; }

    void paint_event(Switch& self, const QPaintEvent& event) {
        const auto rect = extract_rect(self.rect(), 13, 8);

        // 外轮廓相关变量计算
        const auto hover_radius = std::min<double>(rect.width(), rect.height()) / 2;

        const auto outline_radius = hover_radius * 4 / 5;
        const auto outline_width  = outline_radius / 8;
        const auto outline_error  = hover_radius - outline_radius;
        const auto outline_rect   = rect.adjusted( //
            outline_error, outline_error, -outline_error, -outline_error);

        // 计算 handle 半径
        const auto handle_radius_checked   = outline_radius / 4 * 3;
        const auto handle_radius_unchecked = outline_radius / 2 * 1;

        const auto handle_radius =
            handle_radius_unchecked + *position * (handle_radius_checked - handle_radius_unchecked);

        // 计算 handle 坐标
        const auto handle_point_begin = rect.topLeft() + QPointF { hover_radius, hover_radius };
        const auto handle_point_end =
            rect.topLeft() + QPointF { rect.width() - hover_radius, rect.height() - hover_radius };

        const auto handle_point =
            handle_point_begin + *position * (handle_point_end - handle_point_begin);

        // 选择 hover 颜色
        const auto hover_color =
            (!disabled && hovered) ? (checked ? hover_checked : hover_unchecked) : Qt::transparent;

        auto painter = QPainter { &self };
        util::PainterHelper { painter }
            .set_render_hint(QPainter::RenderHint::Antialiasing)
            .rounded_rectangle(from_vector4(*track), from_vector4(*outline), outline_width,
                outline_rect, outline_radius, outline_radius)
            .ellipse(from_vector4(*handle), Qt::transparent, 0, handle_point, handle_radius,
                handle_radius)
            .ellipse(hover_color, Qt::transparent, 0, handle_point, hover_radius, hover_radius)
            .done();
    }

private:
    auto update_switch_ui(Switch&, bool checked, bool disabled = false) -> void {

        // 添加 Switch 轨道颜色动画
        const auto track_target = disabled
            ? (checked ? track_checked_disabled : track_unchecked_disabled)
            : (checked ? track_checked : track_unchecked);
        track->transition_to(from_color(track_target));

        // 添加 Switch 指示器颜色动画
        const auto handle_target = disabled
            ? (checked ? handle_checked_disabled : handle_unchecked_disabled)
            : (checked ? handle_checked : handle_unchecked);
        handle->transition_to(from_color(handle_target));

        // 添加 Switch 外边框颜色动画
        const auto outline_target = disabled
            ? (checked ? outline_checked_disabled : outline_unchecked_disabled)
            : (checked ? outline_checked : outline_unchecked);
        outline->transition_to(from_color(outline_target));

        // 添加 Switch 运动动画
        const auto position_target = checked ? position_checked : position_unchecked;
        position->transition_to(position_target);
    }
};
