#include "checkbox.hh"

#include "creeper-qt/utility/animation/animatable.hh"
#include "creeper-qt/utility/animation/state/spring.hh"
#include "creeper-qt/utility/animation/state/tween.hh"
#include "creeper-qt/utility/animation/transition.hh"
#include "creeper-qt/utility/painter/container.hh"
#include "creeper-qt/utility/painter/shape.hh"

#include <qline.h>
#include <qpainter.h>
#include <qpainterpath.h>
#include <qpen.h>

#include <cmath>
#include <utility>

using namespace creeper;
using Defaults = Checkbox::Defaults;

struct Graphics {
    struct Mark {
        qt::point origin { };
        qt::size size { };
        QPainterPath path;
        qt::color color = Qt::black;
        qt::real width  = Defaults::kStrokeWidth.value;

        Mark(const qt::size& size, QPainterPath path, const qt::color& color, qt::real width)
            : size(size)
            , path(std::move(path))
            , color(color)
            , width(width) { }

        auto operator()(qt::painter& painter) const noexcept -> void {
            painter.save();
            painter.setRenderHint(QPainter::Antialiasing);
            painter.translate(origin);
            painter.setBrush(Qt::NoBrush);
            painter.setPen(QPen { color, width, Qt::SolidLine, Qt::SquareCap });
            painter.drawPath(path);
            painter.restore();
        }
    };

    static auto lerp_color(const qt::color& a, const qt::color& b, double t) -> qt::color {
        t = std::clamp(t, 0.0, 1.0);
        return QColor::fromRgbF(a.redF() + (b.redF() - a.redF()) * t,
            a.greenF() + (b.greenF() - a.greenF()) * t, a.blueF() + (b.blueF() - a.blueF()) * t,
            a.alphaF() + (b.alphaF() - a.alphaF()) * t);
    }

    static auto outer_rect(const qt::point& origin, double t) -> qt::rect {
        const auto inset = 1.0 - std::abs(t - 0.5) * 2.0;
        const auto size  = Defaults::kContainerSize.value - inset * Defaults::kStrokeWidth.value;
        return qt::rect { origin.x() + inset, origin.y() + inset, size, size };
    }

    static auto color_at(const qt::color& inactive, const qt::color& active, double t)
        -> qt::color {
        return t >= 0.25 ? active : lerp_color(inactive, active, t * 4.0);
    }

    static auto check_path(double t) -> QPainterPath {
        const auto start = qt::point { Defaults::kContainerSize.value * 0.25,
            Defaults::kContainerSize.value * 0.50 };
        const auto mid   = qt::point { Defaults::kContainerSize.value * 0.40,
            Defaults::kContainerSize.value * 0.65 };
        const auto end   = qt::point { Defaults::kContainerSize.value * 0.75,
            Defaults::kContainerSize.value * 0.30 };

        auto path = QPainterPath { };
        if (t < 0.5) {
            path.moveTo(start);
            path.lineTo(start + (mid - start) * (t * 2.0));
        } else {
            path.moveTo(start);
            path.lineTo(mid);
            path.lineTo(mid + (end - mid) * ((t - 0.5) * 2.0));
        }
        return path;
    }

    static auto dash_path(double t) -> QPainterPath {
        const auto start = qt::point { Defaults::kContainerSize.value * 0.25,
            Defaults::kContainerSize.value * 0.5 };
        const auto mid   = qt::point { Defaults::kContainerSize.value * 0.50,
            Defaults::kContainerSize.value * 0.5 };
        const auto end   = qt::point { Defaults::kContainerSize.value * 0.75,
            Defaults::kContainerSize.value * 0.5 };

        auto path = QPainterPath { };
        path.moveTo(start + (mid - start) * (1.0 - t));
        path.lineTo(mid + (end - mid) * t);
        return path;
    }
};

struct Checkbox::Impl {
    using Spring = TransitionValue<SpringState<double>>;
    using Tween  = TransitionValue<TweenState<double>>;

    Checkbox& self;
    Animatable animatable;

    CheckState check_state    = CheckState::UNSELECTED;
    CheckState previous_state = CheckState::UNSELECTED;
    bool disabled             = false;
    bool error                = false;

    Measurements measurements;
    Colors colors;

    std::unique_ptr<Spring> position;
    std::unique_ptr<Tween> reaction;
    std::unique_ptr<Tween> hover_fade;
    std::unique_ptr<Tween> focus_fade;

    explicit Impl(Checkbox& self) noexcept
        : self { self }
        , animatable { self } {
        position   = make_transition(animatable, Spring::make_state());
        reaction   = make_transition(animatable, Tween::make_state());
        hover_fade = make_transition(animatable, Tween::make_state());
        focus_fade = make_transition(animatable, Tween::make_state());

        self.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        set_measurements({ });

        position->get_state().config = {
            .k = Defaults::kSpringDefaultSpatialStiffness,
            .d = 2 * Defaults::kSpringDefaultSpatialDamping
                * std::sqrt(Defaults::kSpringDefaultSpatialStiffness),
            .epsilon = 1e-4,
        };
    }

    auto checkmark_color(CheckState state) const -> QColor {
        if (disabled) return colors.disabled_checkmark_color;
        if (error) {
            return state == CheckState::UNSELECTED ? colors.unchecked_checkmark_color
                                                   : colors.error_checked_checkmark_color;
        }
        return state == CheckState::UNSELECTED ? colors.unchecked_checkmark_color
                                               : colors.checked_checkmark_color;
    }

    auto box_color(CheckState state) const -> QColor {
        if (disabled) {
            if (state == CheckState::SELECTED) return colors.disabled_checked_box_color;
            if (state == CheckState::INDETERMINATE) return colors.disabled_indeterminate_box_color;
            return colors.disabled_unchecked_box_color;
        }
        if (error && state != CheckState::UNSELECTED) return colors.error_checked_box_color;
        return state == CheckState::UNSELECTED ? colors.unchecked_box_color
                                               : colors.checked_box_color;
    }

    auto border_color(CheckState state) const -> QColor {
        if (disabled) {
            if (state == CheckState::SELECTED) return colors.disabled_border_color;
            if (state == CheckState::INDETERMINATE)
                return colors.disabled_indeterminate_border_color;
            return colors.disabled_unchecked_border_color;
        }
        if (error) {
            return state == CheckState::UNSELECTED ? colors.error_unchecked_border_color
                                                   : colors.error_checked_border_color;
        }
        return state == CheckState::UNSELECTED ? colors.unchecked_border_color
                                               : colors.checked_border_color;
    }

    auto update_check_ui() const -> void {
        const auto on = check_state != CheckState::UNSELECTED;

        if (check_state == CheckState::INDETERMINATE) {
            position->snap_to(0.0);
            position->transition_to(1.0);
        } else {
            position->transition_to(on ? 1.0 : 0.0);
        }
    }

    auto set_check_state(CheckState state) -> void {
        if (check_state == state) return;

        previous_state = check_state;
        check_state    = state;
        update_check_ui();

        self.update();
        Q_EMIT self.checkStateChanged(check_state);
    }

    auto paint_event() -> void {
        const auto center = qt::point { self.width() / 2.0, self.height() / 2.0 };
        const auto origin = center
            - qt::point { Defaults::kContainerSize.value / 2.0,
                  Defaults::kContainerSize.value / 2.0 };

        const auto t_norm  = *position;
        const auto is_off  = [](CheckState s) { return s == CheckState::UNSELECTED; };
        const auto is_null = [](CheckState s) { return s == CheckState::INDETERMINATE; };
        const auto mark_at = [&](CheckState s, double ts) {
            return is_null(s) ? Graphics::dash_path(ts) : Graphics::check_path(ts);
        };

        const auto active_state  = is_off(check_state) ? previous_state : check_state;
        const auto inactive_fill = box_color(CheckState::UNSELECTED);
        const auto active_fill   = box_color(active_state);
        const auto check_color   = checkmark_color(active_state);
        const auto inactive_side = border_color(CheckState::UNSELECTED);
        const auto active_side   = border_color(active_state);

        auto fill          = inactive_fill;
        auto outline_color = inactive_side;
        auto border_width  = Defaults::kStrokeWidth.value;
        auto outer         = Graphics::outer_rect(origin, 1.0);
        auto mark          = QPainterPath { };

        if (is_off(previous_state) || is_off(check_state)) {
            const auto& t = t_norm;

            outer = Graphics::outer_rect(origin, t);
            fill  = Graphics::color_at(inactive_fill, active_fill, t);

            if (t <= 0.5) {
                outline_color = Graphics::lerp_color(inactive_side, active_side, t);
                border_width  = Defaults::kStrokeWidth.value * (1.0 - t);
            } else {
                outline_color = active_side;
                border_width  = 0.;

                const auto ts = (t - 0.5) * 2.0;
                const auto s  = is_off(check_state) ? previous_state : check_state;
                mark          = mark_at(s, ts);
            }
        } else {
            outer = Graphics::outer_rect(origin, 1.0);
            fill  = active_fill;

            outline_color = active_side;
            border_width  = 0.;

            if (t_norm <= 0.5) {
                mark = mark_at(previous_state, 1.0 - t_norm * 2.0);
            } else {
                mark = mark_at(check_state, (t_norm - 0.5) * 2.0);
            }
        }

        const auto scale      = measurements.container_size / Defaults::kContainerSize;
        const auto layer_size = (measurements.state_layer_size * scale).value;

        auto hover_color = colors.state_layer_color;
        hover_color.setAlphaF(Defaults::kHoverStateLayerOpacity * *hover_fade);

        auto press_color = colors.state_layer_color;
        press_color.setAlphaF(*reaction);

        auto ring_color = colors.focus_indicator_color;
        ring_color.setAlphaF(*focus_fade);

        using namespace painter;
        using namespace painter::common::pro;

        auto paint = Paint::Box {
            BoxImpl { self.size(), Qt::AlignCenter },
            Paint::RoundedRectangle {
              Size { qt::size { layer_size, layer_size } },
              Fill { hover_color },
              Radiuses { layer_size / 2.0 },
            },
            Paint::RoundedRectangle {
              Size { qt::size { layer_size, layer_size } },
              Fill { press_color },
              Radiuses { layer_size / 2.0 },
            },
            Paint::RoundedRectangle {
              Size { qt::size { layer_size, layer_size } },
              Outline { ring_color, (measurements.focus_ring_width * scale).value },
              Radiuses { layer_size / 2.0 },
            },
            Paint::RoundedRectangle {
              Size { qt::size { outer.width() * scale, outer.height() * scale } },
              Fill { fill },
              Outline { outline_color, static_cast<qt::real>(border_width) },
              Radiuses { measurements.container_shape.value },
            },
            Graphics::Mark {
              qt::size {
                Defaults::kContainerSize.value,
                Defaults::kContainerSize.value,
              },
              mark,
              check_color,
              Defaults::kStrokeWidth.value,
            },
        };

        auto painter = qt::painter { &self };
        paint(painter);
    }

    auto load_color_scheme(const ColorScheme& scheme) -> void {
        Defaults::mapColors(scheme, colors);
        self.update();
    }
    auto set_measurements(const Measurements& value) -> void {
        measurements = value;

        const auto target = measurements.touch_target_size.px();
        self.setMinimumSize(target, target);
        self.updateGeometry();
    }

    auto size_hint() const -> QSize {
        const auto target = measurements.touch_target_size.px();
        return QSize { target, target };
    }

    auto set_colors(const Colors& value) -> void {
        colors = value;
        self.update();
    }

    auto set_checked(bool on) -> void {
        set_check_state(on ? CheckState::SELECTED : CheckState::UNSELECTED);
    }

    auto checked() const -> bool { return check_state == CheckState::SELECTED; }

    auto set_disabled(bool on) -> void {
        if (disabled == on) return;
        disabled = on;
        self.setEnabled(!on);
        self.update();
    }

    auto get_disabled() const -> bool { return disabled; }

    auto set_error(bool on) -> void {
        if (error == on) return;
        error = on;
        self.update();
    }

    auto get_error() const -> bool { return error; }

    auto bind_theme_manager(ThemeManager& manager) -> void {
        manager.appendHandler(
            &self, [this](const ThemeManager& m) { load_color_scheme(m.colorScheme()); });
    }

    auto enter_event() -> void {
        hover_fade->get_state().config = {
            .duration = Defaults::kStateLayerDefaultDuration,
            .easing   = Defaults::kEasingLinearCubicBezier,
        };
        hover_fade->transition_to(1.0);
        self.update();
    }

    auto leave_event() -> void {
        hover_fade->get_state().config = {
            .duration = Defaults::kStateLayerDefaultDuration,
            .easing   = Defaults::kEasingLinearCubicBezier,
        };
        hover_fade->transition_to(0.0);
        self.update();
    }

    auto focus_in_event(QFocusEvent* event) -> void {
        const auto keyboard =
            (event->reason() == Qt::TabFocusReason) || (event->reason() == Qt::BacktabFocusReason);
        focus_fade->get_state().config = {
            .duration = keyboard ? Defaults::kStateLayerFocusInDuration
                                 : Defaults::kStateLayerDefaultDuration,
            .easing   = Defaults::kEasingLinearCubicBezier,
        };
        focus_fade->transition_to(keyboard ? 1.0 : 0.0);
        self.update();
    }

    auto focus_out_event() -> void {
        focus_fade->get_state().config = {
            .duration = Defaults::kStateLayerDefaultDuration,
            .easing   = Defaults::kEasingLinearCubicBezier,
        };
        focus_fade->transition_to(0.0);
        self.update();
    }

    auto mouse_press_event() -> void {
        reaction->get_state().config = {
            .duration = Defaults::kFadeInDuration,
            .easing   = Defaults::kEasingLinearCubicBezier,
        };
        reaction->transition_to(Defaults::kPressedStateLayerOpacity);
        self.update();
    }

    auto mouse_release_event() -> void {
        reaction->get_state().config = {
            .duration = Defaults::kFadeOutDuration,
            .easing   = Defaults::kEasingLinearCubicBezier,
        };
        reaction->transition_to(0.0);
        self.update();
    }
};

Checkbox::Checkbox()
    : pimpl(std::make_unique<Impl>(*this)) { }

Checkbox::~Checkbox() = default;

auto Checkbox::loadColorScheme(const ColorScheme& scheme) -> void {
    pimpl->load_color_scheme(scheme);
}

auto Checkbox::bindThemeManager(ThemeManager& manager) -> void {
    pimpl->bind_theme_manager(manager);
}

auto Checkbox::setMeasurements(const Measurements& measurements) -> void {
    pimpl->set_measurements(measurements);
}

auto Checkbox::setColors(const Colors& colors) -> void { pimpl->set_colors(colors); }

auto Checkbox::checkState() const -> CheckState { return pimpl->check_state; }

auto Checkbox::sizeHint() const -> QSize { return pimpl->size_hint(); }

auto Checkbox::setCheckState(CheckState state) -> void { pimpl->set_check_state(state); }

auto Checkbox::setChecked(bool on) -> void { pimpl->set_checked(on); }

auto Checkbox::checked() const -> bool { return pimpl->checked(); }

auto Checkbox::setDisabled(bool on) -> void { pimpl->set_disabled(on); }

auto Checkbox::disabled() const -> bool { return pimpl->get_disabled(); }

auto Checkbox::setError(bool on) -> void { pimpl->set_error(on); }

auto Checkbox::error() const -> bool { return pimpl->get_error(); }

auto Checkbox::enterEvent(qt::EnterEvent* event) -> void {
    pimpl->enter_event();
    QAbstractButton::enterEvent(event);
}

auto Checkbox::leaveEvent(QEvent* event) -> void {
    pimpl->leave_event();
    QAbstractButton::leaveEvent(event);
}

auto Checkbox::focusInEvent(QFocusEvent* event) -> void {
    pimpl->focus_in_event(event);
    QAbstractButton::focusInEvent(event);
}

auto Checkbox::focusOutEvent(QFocusEvent* event) -> void {
    pimpl->focus_out_event();
    QAbstractButton::focusOutEvent(event);
}

auto Checkbox::mousePressEvent(QMouseEvent* event) -> void {
    pimpl->mouse_press_event();
    QAbstractButton::mousePressEvent(event);
}

auto Checkbox::mouseReleaseEvent(QMouseEvent* event) -> void {
    pimpl->mouse_release_event();
    QAbstractButton::mouseReleaseEvent(event);
}

auto Checkbox::paintEvent(QPaintEvent*) -> void { pimpl->paint_event(); }
