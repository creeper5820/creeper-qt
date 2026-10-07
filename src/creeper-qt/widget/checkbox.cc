#include "checkbox.hh"

using namespace creeper;

struct Checkbox::Impl {
    Checkbox& self;

    CheckState check_state = CheckState::UNSELECTED;

    bool disabled = false;
    bool error    = false;
    bool hovered  = false;
    bool focused  = false;
    bool pressed  = false;

    Measurements measurements;
    Colors colors;

    explicit Impl(Checkbox& self) noexcept
        : self { self } { }

    auto load_color_scheme(const ColorScheme&) -> void { }
    auto set_measurements(const Measurements&) -> void { }
    auto set_check_state(CheckState) -> void { }

    auto enter_event() -> void { }
    auto leave_event() -> void { }
    auto focus_in_event() -> void { }
    auto focus_out_event() -> void { }
    auto mouse_press_event() -> void { }
    auto mouse_release_event() -> void { }

    auto paint_event() -> void { }
};

Checkbox::Checkbox()
    : pimpl(std::make_unique<Impl>(*this)) { }

Checkbox::~Checkbox() = default;

auto Checkbox::loadColorScheme(const ColorScheme&) -> void { }

auto Checkbox::bindThemeManager(ThemeManager&) -> void { }

auto Checkbox::setMeasurements(const Measurements&) -> void { }

auto Checkbox::setColorTokens(const Colors&) -> void { }

auto Checkbox::checkState() const -> CheckState { return pimpl->check_state; }

auto Checkbox::setCheckState(CheckState) -> void { }

auto Checkbox::setChecked(bool) -> void { }

auto Checkbox::checked() const -> bool { return false; }

auto Checkbox::setDisabled(bool) -> void { }

auto Checkbox::disabled() const -> bool { return false; }

auto Checkbox::setError(bool) -> void { }

auto Checkbox::error() const -> bool { return false; }

auto Checkbox::enterEvent(qt::EnterEvent*) -> void { }

auto Checkbox::leaveEvent(QEvent*) -> void { }

auto Checkbox::focusInEvent(QFocusEvent*) -> void { }

auto Checkbox::focusOutEvent(QFocusEvent*) -> void { }

auto Checkbox::mousePressEvent(QMouseEvent*) -> void { }

auto Checkbox::mouseReleaseEvent(QMouseEvent*) -> void { }

auto Checkbox::paintEvent(QPaintEvent*) -> void { }

auto Checkbox::Defaults::mapColors(const ColorScheme& scheme, Colors& colors) -> void {
    constexpr auto kDisabledOpacity = 0.38;

    auto disabled_container = scheme.on_surface;
    disabled_container.setAlphaF(kDisabledOpacity);

    auto disabled_outline = scheme.on_surface;
    disabled_outline.setAlphaF(kDisabledOpacity);

    colors.enabled.checked.checkmark   = scheme.on_primary;
    colors.enabled.checked.box         = scheme.primary;
    colors.enabled.checked.border      = scheme.primary;
    colors.enabled.unchecked.checkmark = Qt::transparent;
    colors.enabled.unchecked.box       = Qt::transparent;
    colors.enabled.unchecked.border    = scheme.on_surface_variant;
    colors.enabled.indeterminate       = colors.enabled.checked;

    colors.disabled.checked.checkmark   = scheme.surface;
    colors.disabled.checked.box         = disabled_container;
    colors.disabled.checked.border      = disabled_container;
    colors.disabled.unchecked.checkmark = Qt::transparent;
    colors.disabled.unchecked.box       = Qt::transparent;
    colors.disabled.unchecked.border    = disabled_outline;
    colors.disabled.indeterminate       = colors.disabled.checked;

    colors.error.checked.checkmark   = scheme.on_error;
    colors.error.checked.box         = scheme.error;
    colors.error.checked.border      = scheme.error;
    colors.error.unchecked.checkmark = Qt::transparent;
    colors.error.unchecked.box       = Qt::transparent;
    colors.error.unchecked.border    = scheme.error;
    colors.error.indeterminate       = colors.error.checked;
}
