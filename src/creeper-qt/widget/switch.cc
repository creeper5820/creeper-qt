#include "switch.impl.hh"

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

auto Switch::setColors(const Colors& colors) -> void { pimpl->set_colors(*this, colors); }

auto Switch::sizeHint() const -> QSize { return pimpl->size_hint(); }

auto Switch::setDisabled(bool on) -> void { pimpl->set_disabled(*this, on); }
auto Switch::disabled() const -> bool { return pimpl->disabled; }

auto Switch::setChecked(bool on) -> void { pimpl->set_checked(*this, on); }
auto Switch::checked() const -> bool { return pimpl->checked; }

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

auto Switch::paintEvent(QPaintEvent* event) -> void { pimpl->paint_event(*this, *event); }
