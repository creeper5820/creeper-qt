#pragma once

#include "icon-button.hh"

#include "creeper-qt/utility/animation/animatable.hh"
#include "creeper-qt/utility/animation/state/pid.hh"
#include "creeper-qt/utility/animation/state/spring.hh"
#include "creeper-qt/utility/animation/transition.hh"
#include "creeper-qt/utility/animation/water-ripple.hh"
#include "creeper-qt/utility/painter/helper.hh"

using namespace creeper::icon_button::internal;

constexpr auto kHoverOpacity = double { 0.1 };
constexpr auto kWaterOpacity = double { 0.4 };
constexpr auto kWidthRatio   = double { 1.25 };
constexpr auto kOutlineWidth = double { 1.5 };
constexpr auto kSquareRatio  = double { 0.5 };

constexpr double kp = 15.0, ki = 0.0, kd = 0.0;
constexpr auto kSpringK = double { 400.0 };
constexpr auto kSpringD = double { 15.0 };

constexpr auto kThreshold1D = double { 1e-1 };
constexpr auto kWaterSpeed  = double { 5.0 };

struct IconButton::Impl {

    bool is_hovered = false;

    QString font_icon {};

    Types types { Types::DEFAULT };
    Shape shape { Shape::DEFAULT_ROUND };
    Color color { Color::DEFAULT_FILLED };
    Width width { Width::DEFAULT };

    QColor container_color            = Qt::white;
    QColor container_color_unselected = Qt::white;
    QColor container_color_selected   = Qt::white;

    QColor outline_color            = Qt::gray;
    QColor outline_color_unselected = Qt::gray;
    QColor outline_color_selected   = Qt::gray;

    QColor icon_color            = Qt::black;
    QColor icon_color_unselected = Qt::black;
    QColor icon_color_selected   = Qt::black;

    QColor hover_color            = Qt::gray;
    QColor hover_color_unselected = Qt::gray;
    QColor hover_color_selected   = Qt::gray;

    QColor water_color = Qt::gray;

    Animatable animatable;
    WaterRippleRenderer water_ripple;

    std::unique_ptr<TransitionValue<SpringState<double>>> now_container_radius;
    std::unique_ptr<TransitionValue<PidState<Eigen::Vector4d>>> now_color_container;
    std::unique_ptr<TransitionValue<PidState<Eigen::Vector4d>>> now_color_icon;
    std::unique_ptr<TransitionValue<PidState<Eigen::Vector4d>>> now_color_outline;

    explicit Impl(IconButton& self) noexcept
        : animatable { self }
        , water_ripple { animatable, kWaterSpeed } {

        {
            auto state = std::make_shared<SpringState<double>>();

            state->config.error_threshold = kThreshold1D;
            state->config.k               = kSpringK;
            state->config.d               = kSpringD;

            now_container_radius = make_transition(animatable, std::move(state));
        }
        {
            constexpr auto make_state = [] {
                auto state = std::make_shared<PidState<Eigen::Vector4d>>();

                state->config.kp = kp;
                state->config.ki = ki;
                state->config.kd = kd;
                return state;
            };
            now_color_container = make_transition(animatable, make_state());
            now_color_icon      = make_transition(animatable, make_state());
            now_color_outline   = make_transition(animatable, make_state());
        }

        QObject::connect(&self, &IconButton::clicked, [this, &self] {
            if (types == Types::DEFAULT) {
                const auto center_point = self.mapFromGlobal(QCursor::pos());
                const auto max_distance = std::max(self.width(), self.height());
                water_ripple.clicked(center_point, max_distance);
            }

            toggle_status();
            update_animation_status(self);
        });
    }

    auto enter_event(IconButton& self, const QEvent& event) {
        self.setCursor(Qt::PointingHandCursor);
        is_hovered = true;
    }

    auto leave_event(IconButton& self, const QEvent& event) { is_hovered = false; }

    auto paint_event(IconButton& self, const QPaintEvent& event) {
        // TODO: 做计算数据缓存优化，特别是 Resize 相关的计算
        const auto icon = self.icon();

        const auto color_container = from_vector4(*now_color_container);
        const auto color_icon      = from_vector4(*now_color_icon);
        const auto color_outline   = from_vector4(*now_color_outline);

        const auto hover_color = is_hovered ? get_hover_color() : Qt::transparent;

        const auto container_radius = *now_container_radius;
        const auto container_rect   = container_rectangle(self);

        auto clip_path = QPainterPath {};
        clip_path.addRoundedRect(container_rect, container_radius, container_radius);

        auto renderer = QPainter { &self };
        util::PainterHelper { renderer }
            .set_render_hint(QPainter::Antialiasing)
            .rounded_rectangle(color_container, color_outline, kOutlineWidth, container_rect,
                container_radius, container_radius)
            .apply(water_ripple.renderer(clip_path, water_color))
            .simple_text(font_icon, self.font(), color_icon, container_rect, Qt::AlignCenter)
            .rounded_rectangle(
                hover_color, Qt::transparent, 0, container_rect, container_radius, container_radius)
            .done();
    }

    auto set_types_type(IconButton& self, Types types) {
        this->types = types;
        update_animation_status(self);
    }

    auto set_color_type(IconButton& self, Color color) {
        this->color = color;
        update_animation_status(self);
    }

    auto set_shape_type(IconButton& self, Shape shape) {
        this->shape = shape;
        update_animation_status(self);
    }

    auto set_width_type(IconButton& self, Width width) {
        this->width = width;
        update_animation_status(self);
    }

    auto set_color_scheme(IconButton& self, const ColorScheme& scheme) {
        switch (color) {
        case Color::DEFAULT_FILLED:
            container_color = scheme.primary;
            icon_color      = scheme.on_primary;
            outline_color   = Qt::transparent;

            container_color_unselected = scheme.surface_container_high;
            icon_color_unselected      = scheme.primary;
            outline_color_unselected   = Qt::transparent;

            container_color_selected = scheme.primary;
            icon_color_selected      = scheme.on_primary;
            outline_color_selected   = Qt::transparent;
            break;
        case Color::TONAL:
            container_color = scheme.secondary_container;
            icon_color      = scheme.on_secondary_container;
            outline_color   = Qt::transparent;

            container_color_unselected = scheme.surface_container_high;
            icon_color_unselected      = scheme.surface_variant;
            outline_color_unselected   = Qt::transparent;

            container_color_selected = scheme.secondary_container;
            icon_color_selected      = scheme.on_secondary_container;
            outline_color_selected   = Qt::transparent;
            break;
        case Color::OUTLINED:
            container_color = Qt::transparent;
            outline_color   = scheme.outline_variant;
            icon_color      = scheme.on_surface_variant;

            container_color_unselected = Qt::transparent;
            outline_color_unselected   = scheme.outline_variant;
            icon_color_unselected      = scheme.surface_variant;

            container_color_selected = scheme.inverse_surface;
            outline_color_selected   = scheme.inverse_surface;
            icon_color_selected      = scheme.inverse_on_surface;
            break;
        case Color::STANDARD:
            container_color = Qt::transparent;
            outline_color   = Qt::transparent;
            icon_color      = scheme.on_surface_variant;

            container_color_unselected = Qt::transparent;
            outline_color_unselected   = Qt::transparent;
            icon_color_unselected      = scheme.on_surface_variant;

            container_color_selected = Qt::transparent;
            outline_color_selected   = Qt::transparent;
            icon_color_selected      = scheme.primary;
            break;
        }

        hover_color = icon_color;
        hover_color.setAlphaF(kHoverOpacity);

        hover_color_selected = icon_color_selected;
        hover_color_selected.setAlphaF(kHoverOpacity);

        hover_color_unselected = icon_color_unselected;
        hover_color_unselected.setAlphaF(kHoverOpacity);

        water_color = icon_color;
        water_color.setAlphaF(kWaterOpacity);

        update_animation_status(self);
    }

    auto load_theme_manager(IconButton& self, ThemeManager& manager) {
        manager.append_handler(&self, [this, &self](const ThemeManager& manager) {
            set_color_scheme(self, manager.color_scheme());
        });
    }

private:
    auto update_animation_status(IconButton& self) -> void {

        const auto container_color_target = (types == Types::DEFAULT) ? container_color : //
            (types == Types::TOGGLE_SELECTED) ? container_color_selected
                                              : container_color_unselected;
        now_color_container->transition_to(from_color(container_color_target));

        const auto icon_color_target = (types == Types::DEFAULT) ? icon_color : //
            (types == Types::TOGGLE_SELECTED) ? icon_color_selected
                                              : icon_color_unselected;
        now_color_icon->transition_to(from_color(icon_color_target));

        const auto outline_color_target //
            = (types == Types::DEFAULT)         ? outline_color
            : (types == Types::TOGGLE_SELECTED) ? outline_color_selected
                                                : outline_color_unselected;
        now_color_outline->transition_to(from_color(outline_color_target));

        const auto rectangle     = container_rectangle(self);
        const auto radius_round  = std::min<double>(rectangle.width(), rectangle.height()) / 2.;
        const auto radius_target = (types == Types::TOGGLE_SELECTED || shape == Shape::SQUARE)
            ? radius_round * kSquareRatio
            : radius_round * 1.0;
        now_container_radius->transition_to(radius_target);
    }

    auto get_hover_color() const noexcept -> QColor {
        switch (types) {
        case Types::DEFAULT:
            return hover_color;
        case Types::TOGGLE_UNSELECTED:
            return hover_color_unselected;
        case Types::TOGGLE_SELECTED:
            return hover_color_selected;
        }
        return { /* 不可能到达的彼岸 */ };
    }

    auto toggle_status() -> void {
        if (types == Types::TOGGLE_UNSELECTED) types = Types::TOGGLE_SELECTED;
        else if (types == Types::TOGGLE_SELECTED) types = Types::TOGGLE_UNSELECTED;
    }

    // 设计指南上的大小全是固定的，十分不自由，故转成比例
    auto container_rectangle(IconButton& self) -> QRectF {
        return (width == Width::DEFAULT) ? (extract_rect(self.rect(), 1, 1))
            : (width == Width::NARROW)   ? (extract_rect(self.rect(), 1, kWidthRatio))
                                         : (extract_rect(self.rect(), kWidthRatio, 1));
    }
};
