#include "icon-button.hh"
#include "creeper-qt/utility/animation/core.hh"
#include "creeper-qt/utility/animation/math.hh"
#include "creeper-qt/utility/animation/motion-system.hh"
#include "creeper-qt/utility/animation/water-ripple.hh"
#include "creeper-qt/utility/painter/helper.hh"

using namespace creeper::icon_button::internal;
using creeper::util::animation::AnimationCore;
using creeper::util::animation::WaterRipple;
using creeper::util::animation::WaterRippleContainer;

struct IconButton::Impl {
    using Track4D = util::animation::FinitePidTracker<Eigen::Vector4d>;

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

    QColor hover_color;
    QColor water_color;

    std::shared_ptr<double> container_radius_current { std::make_shared<double>(0) };

    std::shared_ptr<Eigen::Vector4d> container_color_current { std::make_shared<Eigen::Vector4d>(
        Eigen::Vector4d::Zero()) };
    std::shared_ptr<Eigen::Vector4d> icon_color_current { std::make_shared<Eigen::Vector4d>(
        Eigen::Vector4d::Zero()) };
    std::shared_ptr<Eigen::Vector4d> outline_color_current { std::make_shared<Eigen::Vector4d>(
        Eigen::Vector4d::Zero()) };

    explicit Impl(IconButton& self) noexcept
        : animation_core { [&self] { self.update(); }, 90 } { }

    void enter_event(IconButton& self, const QEvent& event) {
        self.setCursor(Qt::PointingHandCursor);
        is_hovered = true;
    }

    void leave_event(IconButton& self, const QEvent& event) { is_hovered = false; }

    void mouse_relaese_event(IconButton& self, const QMouseEvent& event) { }

    void paint_event(IconButton& self, const QPaintEvent& event) {
        const auto icon = self.icon();

        // 设计指南上的大小全是固定的，十分不自由，故转成比例
        const auto container_rect //
            = (width == Width::DEFAULT) ? (extract_rect(self.rect(), 1, 1))
            : (width == Width::NARROW)  ? (extract_rect(self.rect(), 1, kWidthRatio))
                                        : (extract_rect(self.rect(), kWidthRatio, 1));
        const auto container_radius = std::min(container_rect.width(), container_rect.height()) / 2;

        const auto container_color = from_vector4(*container_color_current);
        const auto icon_color      = from_vector4(*icon_color_current);
        const auto outline_color   = from_vector4(*outline_color_current);

        const auto hover_color = is_hovered ? this->hover_color : Qt::transparent;

        auto renderer = QPainter { &self };
        util::PainterHelper { renderer }
            .set_render_hint(QPainter::Antialiasing)
            .rounded_rectangle(container_color, outline_color, kOutlineWidth, container_rect,
                container_radius, container_radius)
            .simple_text(font_icon, self.font(), icon_color, container_rect, Qt::AlignCenter);

        // TODO: 绘制 Icon
    }

    void set_types_type(Types types) { this->types = types; }

    void set_color_type(Color color) { this->color = color; }

    void set_shape_type(Shape shape) { this->shape = shape; }

    void set_width_type(Width width) { this->width = width; }

    void set_color_scheme(IconButton& self, const ColorScheme& scheme) {
        switch (color) {
        case Color::DEFAULT_FILLED:
            container_color = scheme.primary;
            icon_color      = scheme.on_primary;
            outline_color   = Qt::transparent;

            container_color_unselected = scheme.surface_container_highest;
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

            container_color_unselected = scheme.surface_container_highest;
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

            container_color_selected = Qt::transparent;
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

            container_color_unselected = Qt::transparent;
            outline_color_unselected   = Qt::transparent;
            icon_color_selected        = scheme.primary;
            break;
        }
        update_animation_status();
    }

    void load_theme_manager(IconButton& self, ThemeManager& manager) {
        manager.append_handler(&self, [this, &self](const ThemeManager& manager) {
            set_color_scheme(self, manager.color_scheme());
        });
    }

private:
    void update_animation_status() {

        constexpr double kp = 15.0, ki = 0.0, kd = 0.0, hz = 90;

        const auto stop_token = std::make_shared<bool>(false);

        const auto container_color_target //
            = (types == Types::DEFAULT)         ? container_color
            : (types == Types::TOGGLE_SELECTED) ? container_color_selected
                                                : container_color_unselected;
        animation_core.append(std::make_unique<Track4D>(container_color_current,
            from_color(container_color_target), stop_token, kp, ki, kd, hz));

        const auto icon_color_target //
            = (types == Types::DEFAULT)         ? icon_color
            : (types == Types::TOGGLE_SELECTED) ? icon_color_selected
                                                : icon_color_unselected;
        animation_core.append(std::make_unique<Track4D>(
            icon_color_current, from_color(icon_color_target), stop_token, kp, ki, kd, hz));

        const auto outline_color_target //
            = (types == Types::DEFAULT)         ? outline_color
            : (types == Types::TOGGLE_SELECTED) ? outline_color_selected
                                                : outline_color_unselected;
        animation_core.append(std::make_unique<Track4D>(
            outline_color_current, from_color(outline_color_target), stop_token, kp, ki, kd, hz));

        for (auto& stop_token : stop_tokens)
            *stop_token = true;

        stop_tokens.clear();
        stop_tokens.push_back(std::move(stop_token));
    }
};

IconButton::IconButton()
    : pimpl(std::make_unique<Impl>(*this)) { }

IconButton::~IconButton() = default;

void IconButton::set_color_scheme(const ColorScheme& scheme) {
    pimpl->set_color_scheme(*this, scheme);
}
void IconButton::load_theme_manager(ThemeManager& manager) {
    pimpl->load_theme_manager(*this, manager);
}

void IconButton::enterEvent(QEvent* event) {
    pimpl->enter_event(*this, *event);
    QAbstractButton::enterEvent(event);
}
void IconButton::leaveEvent(QEvent* event) {
    pimpl->leave_event(*this, *event);
    QAbstractButton::leaveEvent(event);
}

void IconButton::mouseReleaseEvent(QMouseEvent* event) {
    pimpl->mouse_relaese_event(*this, *event);
    QAbstractButton::mouseReleaseEvent(event);
}

void IconButton::paintEvent(QPaintEvent* event) { pimpl->paint_event(*this, *event); }

void IconButton::set_icon(const QString& icon) { pimpl->font_icon = icon; }
void IconButton::set_icon(const QIcon& icon) { QAbstractButton::setIcon(icon); }

void IconButton::set_types(Types types) { pimpl->set_types_type(types); }
void IconButton::set_shape(Shape shape) { pimpl->set_shape_type(shape); }
void IconButton::set_color(Color color) { pimpl->set_color_type(color); }
void IconButton::set_width(Width width) { pimpl->set_width_type(width); }
