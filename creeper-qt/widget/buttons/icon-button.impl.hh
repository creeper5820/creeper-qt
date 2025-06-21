#pragma once

#include "creeper-qt/utility/animation/core.hh"
#include "creeper-qt/utility/animation/math.hh"
#include "creeper-qt/utility/animation/motion-system.hh"
#include "creeper-qt/utility/animation/water-ripple.hh"
#include "creeper-qt/utility/painter/helper.hh"
#include "icon-button.hh"

using namespace creeper::icon_button::internal;
using creeper::util::animation::AnimationCore;
using creeper::util::animation::WaterRipple;
using creeper::util::animation::WaterRippleContainer;

constexpr auto kHoverOpacity = double { 0.1 };
constexpr auto kWaterOpacity = double { 0.4 };
constexpr auto kWidthRatio   = double { 1.25 };
constexpr auto kOutlineWidth = double { 1.5 };
constexpr auto kSquareRatio  = double { 0.5 };

constexpr double kp = 15.0, ki = 0.0, kd = 0.0;
constexpr auto kSpringK = double { 400.0 };
constexpr auto kSpringD = double { 15.0 };

constexpr auto kAnimationHZ = int { 90 };
constexpr auto kThreshold4D = double { 1.0 };
constexpr auto kThreshold1D = double { 1e-1 };
constexpr auto kWaterSpeed  = double { 5.0 };

struct IconButton::Impl {
    using Track4D = util::animation::FinitePidTracker<Eigen::Vector4d>;
    using Track1D = util::animation::FiniteSringTracker<double>;

    AnimationCore animation_core;
    WaterRippleContainer water_ripples;
    std::vector<std::shared_ptr<bool>> stop_tokens;

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

    std::shared_ptr<double> container_radius_current { std::make_shared<double>(0) };

    std::shared_ptr<Eigen::Vector4d> container_color_current { std::make_shared<Eigen::Vector4d>(
        Eigen::Vector4d::Zero()) };
    std::shared_ptr<Eigen::Vector4d> icon_color_current { std::make_shared<Eigen::Vector4d>(
        Eigen::Vector4d::Zero()) };
    std::shared_ptr<Eigen::Vector4d> outline_color_current { std::make_shared<Eigen::Vector4d>(
        Eigen::Vector4d::Zero()) };

    explicit Impl(IconButton& self) noexcept
        : animation_core { [&self] { self.update(); }, kAnimationHZ } {

        QObject::connect(&self, &IconButton::clicked, [this, &self] {
            if (types == Types::DEFAULT) {
                const auto center_point = self.mapFromGlobal(QCursor::pos());
                const auto max_distance = std::max(self.width(), self.height());

                auto container_outline_path = QPainterPath {};
                container_outline_path.addRoundedRect(container_rectangle(self),

                    *container_radius_current, *container_radius_current);
                animation_core.append(std::make_unique<WaterRipple>(water_color,
                    container_outline_path, center_point, kWaterSpeed, max_distance,
                    [this](std::unique_ptr<WaterRipple::Result> result) {
                        return water_ripples.append(std::move(result)), false;
                    }));
            }

            toggle_status();
            update_animation_status(self);
        });
    }

    void enter_event(IconButton& self, const QEvent& event) {
        self.setCursor(Qt::PointingHandCursor);
        is_hovered = true;
    }

    void leave_event(IconButton& self, const QEvent& event) { is_hovered = false; }

    void paint_event(IconButton& self, const QPaintEvent& event) {
        // TODO: 做计算数据缓存优化，特别是 Resize 相关的计算
        const auto icon = self.icon();

        const auto container_color = from_vector4(*container_color_current);
        const auto icon_color      = from_vector4(*icon_color_current);
        const auto outline_color   = from_vector4(*outline_color_current);

        const auto hover_color = is_hovered ? get_hover_color() : Qt::transparent;

        const auto container_radius = *container_radius_current;
        const auto container_rect   = container_rectangle(self);

        const auto render_water_ripple = [this, &self](QPainter& painter) {
            water_ripples.render(painter, self.rect(), self.rect());
        };

        auto renderer = QPainter { &self };
        util::PainterHelper { renderer }
            .set_render_hint(QPainter::Antialiasing)
            .rounded_rectangle(container_color, outline_color, kOutlineWidth, container_rect,
                container_radius, container_radius)
            .apply(render_water_ripple)
            .simple_text(font_icon, self.font(), icon_color, container_rect, Qt::AlignCenter)
            .rounded_rectangle(
                hover_color, Qt::transparent, 0, container_rect, container_radius, container_radius)
            .done();
    }

    void set_types_type(IconButton& self, Types types) {
        this->types = types;
        update_animation_status(self);
    }

    void set_color_type(IconButton& self, Color color) {
        this->color = color;
        update_animation_status(self);
    }

    void set_shape_type(IconButton& self, Shape shape) {
        this->shape = shape;
        update_animation_status(self);
    }

    void set_width_type(IconButton& self, Width width) {
        this->width = width;
        update_animation_status(self);
    }

    void set_color_scheme(IconButton& self, const ColorScheme& scheme) {
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

    void load_theme_manager(IconButton& self, ThemeManager& manager) {
        manager.append_handler(&self, [this, &self](const ThemeManager& manager) {
            set_color_scheme(self, manager.color_scheme());
        });
    }

private:
    void update_animation_status(IconButton& self) {
        const auto stop_token = std::make_shared<bool>(false);

        const auto container_color_target //
            = (types == Types::DEFAULT)         ? container_color
            : (types == Types::TOGGLE_SELECTED) ? container_color_selected
                                                : container_color_unselected;

        animation_core.append(std::make_unique<Track4D>(container_color_current, //
            from_color(container_color_target), stop_token, kp, ki, kd, kAnimationHZ,
            kThreshold4D));

        const auto icon_color_target //
            = (types == Types::DEFAULT)         ? icon_color
            : (types == Types::TOGGLE_SELECTED) ? icon_color_selected
                                                : icon_color_unselected;
        animation_core.append(std::make_unique<Track4D>(icon_color_current,
            from_color(icon_color_target), stop_token, kp, ki, kd, kAnimationHZ, kThreshold4D));

        const auto outline_color_target //
            = (types == Types::DEFAULT)         ? outline_color
            : (types == Types::TOGGLE_SELECTED) ? outline_color_selected
                                                : outline_color_unselected;
        animation_core.append(std::make_unique<Track4D>(outline_color_current,
            from_color(outline_color_target), stop_token, kp, ki, kd, kAnimationHZ, kThreshold4D));

        const auto radius_round  = std::min<double>(self.width(), self.height()) / 2.;
        const auto radius_target = (types == Types::TOGGLE_SELECTED || shape == Shape::SQUARE)
            ? radius_round * kSquareRatio
            : radius_round * 1.0;
        animation_core.append(std::make_unique<Track1D>(container_radius_current, radius_target,
            stop_token, kSpringK, kSpringD, kAnimationHZ, kThreshold1D));

        for (auto& stop_token : stop_tokens)
            *stop_token = true;

        stop_tokens.clear();
        stop_tokens.push_back(std::move(stop_token));
    }

    QColor get_hover_color() const noexcept {
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

    void toggle_status() {
        if (types == Types::TOGGLE_UNSELECTED) types = Types::TOGGLE_SELECTED;
        else if (types == Types::TOGGLE_SELECTED) types = Types::TOGGLE_UNSELECTED;
    }

    // 设计指南上的大小全是固定的，十分不自由，故转成比例
    QRectF container_rectangle(IconButton& self) {
        return (width == Width::DEFAULT) ? (extract_rect(self.rect(), 1, 1))
            : (width == Width::NARROW)   ? (extract_rect(self.rect(), 1, kWidthRatio))
                                         : (extract_rect(self.rect(), kWidthRatio, 1));
    }
};
