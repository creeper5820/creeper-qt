#pragma once

#include "widget/switch.hh"

#include <qpainter.h>

#include "utility/animation/motion-system.hh"
#include "utility/painter/helper.hh"

using namespace creeper::_switch::internal;
using creeper::util::animation::AnimationCore;

struct Switch::Impl {
    AnimationCore animation_core;
    std::vector<std::shared_ptr<bool>> stop_tokens;

    bool checked  = false;
    bool disabled = false;

    std::shared_ptr<Eigen::Vector4d> track { std::make_shared<Eigen::Vector4d>() };
    QColor track_unchecked;
    QColor track_checked;
    QColor track_disabled;

    std::shared_ptr<Eigen::Vector4d> handle { std::make_shared<Eigen::Vector4d>() };
    QColor handle_unchecked;
    QColor handle_checked;
    QColor handle_disabled;

    std::shared_ptr<Eigen::Vector4d> outline { std::make_shared<Eigen::Vector4d>() };
    QColor outline_unchecked;
    QColor outline_checked;
    QColor outline_disabled;

    std::shared_ptr<Eigen::Vector4d> hover { std::make_shared<Eigen::Vector4d>() };
    QColor hover_unchecked;
    QColor hover_checked;

    std::shared_ptr<double> position { std::make_shared<double>(0) };
    static constexpr double position_unchecked = 0.0;
    static constexpr double position_checked   = 1.0;

    explicit Impl(Switch& self)
        : animation_core([&self] { self.update(); }, 90) {
        QObject::connect(&self, &QAbstractButton::clicked, [this] { set_checked(!checked); });
    }

    void set_color_scheme(const ColorScheme& scheme) {
        track_unchecked = scheme.surface_variant;
        track_checked   = scheme.primary;

        handle_unchecked = scheme.outline;
        handle_checked   = scheme.on_primary;

        outline_unchecked = scheme.outline;
        outline_checked   = scheme.primary;

        track_disabled   = scheme.surface;
        handle_disabled  = scheme.on_surface;
        outline_disabled = scheme.on_surface;

        update_switch_ui(checked);
    }

    void set_disabled(bool on) { }

    void set_checked(bool on) {
        if (disabled || checked == on) return;

        update_switch_ui(on);

        checked = on;
    }

    void enter_event(Switch& self, const QEvent& event) { }

    void leave_event(Switch& self, const QEvent& event) { }

    void paint_event(Switch& self, const QPaintEvent& event) {
        auto painter = QPainter { &self };

        const auto rect = self.rect();

        const auto outline_radius = std::min<double>(rect.width(), rect.height()) / 2;
        const auto outline_width  = outline_radius / 16;

        const auto handle_radius_checked   = outline_radius / 3 * 2;
        const auto handle_radius_unchecked = outline_radius / 2 * 1;

        const auto radius_error  = *position * (handle_radius_checked - handle_radius_unchecked);
        const auto handle_radius = handle_radius_unchecked + radius_error;

        util::PainterHelper { painter }
            .set_render_hint(QPainter::RenderHint::Antialiasing)
            .rounded_rectangle(from_vector4(*track), from_vector4(*outline), outline_width, rect,
                outline_radius, outline_radius)
            .done();
    }

private:
    void update_switch_ui(bool checked, bool disabled = false) {
        using Tracker4D = util::animation::FinitePidTracker<Eigen::Vector4d>;
        using Tracker1D = util::animation::FinitePidTracker<double>;

        constexpr double kp = 20.0, ki = 0.0, kd = 0.0, hz = 90;

        const auto stop_token = std::make_shared<bool>(false);

        // 添加 Switch 轨道颜色动画
        const auto track_target = disabled ? track_disabled : //
            (checked ? track_checked : track_unchecked);
        const auto track_animation =
            Tracker4D { track, from_color(track_target), stop_token, kp, ki, kd, hz };
        animation_core.append(std::make_unique<Tracker4D>(std::move(track_animation)));

        // 添加 Switch 指示器颜色动画
        const auto handle_target = disabled ? handle_disabled : //
            (checked ? handle_checked : handle_unchecked);
        const auto handle_animation =
            Tracker4D { handle, from_color(handle_target), stop_token, kp, ki, kd, hz };
        animation_core.append(std::make_unique<Tracker4D>(std::move(handle_animation)));

        // 添加 Switch 外边框颜色动画
        const auto outline_target = disabled ? outline_disabled : //
            (checked ? outline_checked : outline_unchecked);
        const auto outline_animation =
            Tracker4D { outline, from_color(outline_target), stop_token, kp, ki, kd, hz };
        animation_core.append(std::make_unique<Tracker4D>(std::move(outline_animation)));

        // 添加 Switch 运动动画
        const auto position_target = checked ? position_checked : position_unchecked;
        const auto position_animation =
            Tracker1D { position, position_target, stop_token, kp, ki, kd, hz };
        animation_core.append(std::make_unique<Tracker1D>(std::move(position_animation)));

        // 打断动画，将 token 设为 true 后清空，动画类持有的 token 会随着动画的结束而释放
        for (auto& stop_token : stop_tokens)
            *stop_token = true;

        stop_tokens.clear();
        stop_tokens.push_back(std::move(stop_token));
    }
};
