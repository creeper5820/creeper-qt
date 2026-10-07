#include "checkbox.hh"

#include "creeper-qt/utility/animation/animatable.hh"
#include "creeper-qt/utility/animation/state/tween.hh"
#include "creeper-qt/utility/animation/transition.hh"
#include "creeper-qt/utility/painter/container.hh"
#include "creeper-qt/utility/painter/shape.hh"

#include <qline.h>
#include <qpainter.h>
#include <qpainterpath.h>
#include <qpen.h>

#include <utility>

using namespace creeper;
using Defaults = Checkbox::Defaults;

struct Graphics {
    struct Mark {
        qt::point origin { };
        qt::size size { };
        QPainterPath path;
        qt::color color = Qt::black;
        qt::real width  = Defaults::kStrokeWidth;

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
        const auto size  = Defaults::kContainerSize - inset * Defaults::kStrokeWidth;
        return qt::rect { origin.x() + inset, origin.y() + inset, size, size };
    }

    static auto color_at(const qt::color& inactive, const qt::color& active, double t)
        -> qt::color {
        return t >= 0.25 ? active : lerp_color(inactive, active, t * 4.0);
    }

    static auto check_path(double t) -> QPainterPath {
        const auto start =
            qt::point { Defaults::kContainerSize * 0.25, Defaults::kContainerSize * 0.50 };
        const auto mid =
            qt::point { Defaults::kContainerSize * 0.40, Defaults::kContainerSize * 0.65 };
        const auto end =
            qt::point { Defaults::kContainerSize * 0.75, Defaults::kContainerSize * 0.30 };

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
        const auto start =
            qt::point { Defaults::kContainerSize * 0.25, Defaults::kContainerSize * 0.5 };
        const auto mid =
            qt::point { Defaults::kContainerSize * 0.50, Defaults::kContainerSize * 0.5 };
        const auto end =
            qt::point { Defaults::kContainerSize * 0.75, Defaults::kContainerSize * 0.5 };

        auto path = QPainterPath { };
        path.moveTo(start + (mid - start) * (1.0 - t));
        path.lineTo(mid + (end - mid) * t);
        return path;
    }
};

struct Checkbox::Impl {
    using Tween = TransitionValue<TweenState<double>>;

    Checkbox& self;
    Animatable animatable;

    CheckState check_state    = CheckState::UNSELECTED;
    CheckState previous_state = CheckState::UNSELECTED;
    bool disabled             = false;
    bool error                = false;

    Measurements measurements;
    Colors colors;

    std::unique_ptr<Tween> position;
    std::unique_ptr<Tween> reaction;
    std::unique_ptr<Tween> hover_fade;
    std::unique_ptr<Tween> focus_fade;

    explicit Impl(Checkbox& self) noexcept
        : self { self }
        , animatable { self } {
        position   = make_transition(animatable, Tween::make_state());
        reaction   = make_transition(animatable, Tween::make_state());
        hover_fade = make_transition(animatable, Tween::make_state());
        focus_fade = make_transition(animatable, Tween::make_state());

        self.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        set_measurements({ });

        position->get_state().config = {
            .duration = Defaults::kToggleDuration,
            .easing   = Defaults::kEaseIn,
        };
        reaction->get_state().config = {
            .duration = Defaults::kReactionDuration,
            .easing   = Defaults::kFastOutSlowIn,
        };
        hover_fade->get_state().config = {
            .duration = Defaults::kReactionFade,
            .easing   = Defaults::kFastOutSlowIn,
        };
        focus_fade->get_state().config = {
            .duration = Defaults::kReactionFade,
            .easing   = Defaults::kFastOutSlowIn,
        };
    }

    auto group() const -> const Colors::CheckStateTokens& {
        if (disabled) return colors.disabled;
        if (error) return colors.error;
        return colors.enabled;
    }

    auto update_check_ui() const -> void {
        const auto on = check_state != CheckState::UNSELECTED;

        position->get_state().config = {
            .duration = Defaults::kToggleDuration,
            .easing   = on ? Defaults::kEaseIn : Defaults::kEaseOut,
        };

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
        const auto origin =
            center - qt::point { Defaults::kContainerSize / 2.0, Defaults::kContainerSize / 2.0 };

        const auto& state_colors = group();
        const auto inactive_fill = state_colors.unchecked.box;
        const auto active_fill   = state_colors.checked.box;
        const auto check_color   = state_colors.checked.checkmark;
        const auto inactive_side = state_colors.unchecked.border;
        const auto active_side   = state_colors.checked.border;

        const auto t_norm  = *position;
        const auto is_off  = [](CheckState s) { return s == CheckState::UNSELECTED; };
        const auto is_null = [](CheckState s) { return s == CheckState::INDETERMINATE; };
        const auto mark_at = [&](CheckState s, double ts) {
            return is_null(s) ? Graphics::dash_path(ts) : Graphics::check_path(ts);
        };

        auto fill         = inactive_fill;
        auto border_color = inactive_side;
        auto border_width = Defaults::kStrokeWidth;
        auto outer        = Graphics::outer_rect(origin, 1.0);
        auto mark         = QPainterPath { };

        if (is_off(previous_state) || is_off(check_state)) {
            const auto& t = t_norm;

            outer = Graphics::outer_rect(origin, t);
            fill  = Graphics::color_at(inactive_fill, active_fill, t);

            if (t <= 0.5) {
                border_color = Graphics::lerp_color(inactive_side, active_side, t);
                border_width = Defaults::kStrokeWidth * (1.0 - t);
            } else {
                border_color = active_side;
                border_width = 0.;

                const auto ts = (t - 0.5) * 2.0;
                const auto s  = is_off(check_state) ? previous_state : check_state;
                mark          = mark_at(s, ts);
            }
        } else {
            outer = Graphics::outer_rect(origin, 1.0);
            fill  = active_fill;

            border_color = active_side;
            border_width = 0.;

            if (t_norm <= 0.5) {
                mark = mark_at(previous_state, 1.0 - t_norm * 2.0);
            } else {
                mark = mark_at(check_state, (t_norm - 0.5) * 2.0);
            }
        }

        const auto scale =
            static_cast<double>(measurements.container_size) / Defaults::kContainerSize;
        const auto layer_size = 2 * Defaults::kSplashRadius * scale;

        auto hover_color = state_colors.unchecked.state_layer;
        hover_color.setAlphaF(0.08 * *hover_fade);

        auto press_color = state_colors.unchecked.state_layer;
        press_color.setAlphaF(*reaction);

        auto ring_color = colors.focus_ring;
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
              Outline { ring_color, Defaults::kFocusRingWidth * scale },
              Radiuses { layer_size / 2.0 },
            },
            Paint::RoundedRectangle {
              Size { qt::size { outer.width() * scale, outer.height() * scale } },
              Fill { fill },
              Outline { border_color, static_cast<qt::real>(border_width) },
              Radiuses { static_cast<double>(measurements.container_shape) },
            },
            Graphics::Mark {
              qt::size {
                Defaults::kContainerSize,
                Defaults::kContainerSize,
              },
              mark,
              check_color,
              Defaults::kStrokeWidth,
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

        const auto target = measurements.touch_target_size;
        self.setMinimumSize(target, target);
        self.updateGeometry();
    }

    auto size_hint() const -> QSize {
        const auto target = measurements.touch_target_size;
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
        hover_fade->transition_to(1.0);
        self.update();
    }

    auto leave_event() -> void {
        hover_fade->transition_to(0.0);
        self.update();
    }

    auto focus_in_event(QFocusEvent* event) -> void {
        const auto keyboard =
            (event->reason() == Qt::TabFocusReason) || (event->reason() == Qt::BacktabFocusReason);
        focus_fade->transition_to(keyboard ? 1.0 : 0.0);
        self.update();
    }

    auto focus_out_event() -> void {
        focus_fade->transition_to(0.0);
        self.update();
    }

    auto mouse_press_event() -> void {
        reaction->transition_to(Defaults::kRadialReactionAlpha);
        self.update();
    }

    auto mouse_release_event() -> void {
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

auto Checkbox::Defaults::mapColors(const ColorScheme& scheme, Colors& colors) -> void {
    constexpr auto kDisabledOpacity = 0.38;

    auto disabled_container = scheme.on_surface;
    disabled_container.setAlphaF(kDisabledOpacity);

    auto disabled_outline = scheme.on_surface;
    disabled_outline.setAlphaF(kDisabledOpacity);

    colors.enabled.checked.checkmark   = scheme.on_primary;
    colors.enabled.checked.box         = scheme.primary;
    colors.enabled.checked.border      = scheme.primary;
    colors.enabled.checked.state_layer = scheme.primary;

    colors.enabled.unchecked.checkmark   = Qt::transparent;
    colors.enabled.unchecked.box         = Qt::transparent;
    colors.enabled.unchecked.border      = scheme.on_surface_variant;
    colors.enabled.unchecked.state_layer = scheme.on_surface;

    colors.enabled.indeterminate = colors.enabled.checked;

    colors.disabled.checked.checkmark   = scheme.surface;
    colors.disabled.checked.box         = disabled_container;
    colors.disabled.checked.border      = disabled_container;
    colors.disabled.checked.state_layer = scheme.on_surface;

    colors.disabled.unchecked.checkmark   = Qt::transparent;
    colors.disabled.unchecked.box         = Qt::transparent;
    colors.disabled.unchecked.border      = disabled_outline;
    colors.disabled.unchecked.state_layer = scheme.on_surface;

    colors.disabled.indeterminate = colors.disabled.checked;

    colors.error.checked.checkmark   = scheme.on_error;
    colors.error.checked.box         = scheme.error;
    colors.error.checked.border      = scheme.error;
    colors.error.checked.state_layer = scheme.error;

    colors.error.unchecked.checkmark   = Qt::transparent;
    colors.error.unchecked.box         = Qt::transparent;
    colors.error.unchecked.border      = scheme.error;
    colors.error.unchecked.state_layer = scheme.error;

    colors.error.indeterminate = colors.error.checked;

    colors.focus_ring = scheme.secondary;
}
