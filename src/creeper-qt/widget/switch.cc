#include "switch.impl.hh"

Switch::Switch()
    : pimpl(std::make_unique<Impl>(*this)) { }

Switch::~Switch() = default;

auto Switch::loadColorScheme(const ColorScheme& scheme) -> void {
    pimpl->loadColorScheme(*this, scheme), update();
}

auto Switch::bindThemeManager(ThemeManager& manager) -> void {
    manager.appendHandler(
        this, [this](const ThemeManager& manager) { loadColorScheme(manager.colorScheme()); });
}

auto Switch::setDisabled(bool on) -> void { pimpl->set_disabled(*this, on); }
auto Switch::disabled() const -> bool { return pimpl->disabled; }

auto Switch::setChecked(bool on) -> void { pimpl->set_checked(*this, on); }
auto Switch::checked() const -> bool { return pimpl->checked; }

auto Switch::setTrackColorUnchecked(const QColor& color) -> void { pimpl->track_unchecked = color; }
auto Switch::setTrackColorChecked(const QColor& color) -> void { pimpl->track_checked = color; }
auto Switch::setTrackColorUncheckedDisabled(const QColor& color) -> void {
    pimpl->track_unchecked_disabled = color;
}
auto Switch::setTrackColorCheckedDisabled(const QColor& color) -> void {
    pimpl->track_checked_disabled = color;
}

auto Switch::setHandleColorUnchecked(const QColor& color) -> void {
    pimpl->handle_unchecked = color;
}
auto Switch::setHandleColorChecked(const QColor& color) -> void { pimpl->handle_checked = color; }
auto Switch::setHandleColorUncheckedDisabled(const QColor& color) -> void {
    pimpl->handle_unchecked_disabled = color;
}
auto Switch::setHandleColorCheckedDisabled(const QColor& color) -> void {
    pimpl->handle_checked_disabled = color;
}

auto Switch::setOutlineColorUnchecked(const QColor& color) -> void {
    pimpl->outline_unchecked = color;
}
auto Switch::setOutlineColorChecked(const QColor& color) -> void { pimpl->outline_checked = color; }
auto Switch::setOutlineColorUncheckedDisabled(const QColor& color) -> void {
    pimpl->outline_unchecked_disabled = color;
}
auto Switch::setOutlineColorCheckedDisabled(const QColor& color) -> void {
    pimpl->outline_checked_disabled = color;
}

auto Switch::setHoverColorUnchecked(const QColor& color) -> void { pimpl->hover_unchecked = color; }
auto Switch::setHoverColorChecked(const QColor& color) -> void { pimpl->hover_checked = color; }

auto Switch::enterEvent(qt::EnterEvent* event) -> void {
    pimpl->enter_event(*this, *event);
    QAbstractButton::enterEvent(event);
}

auto Switch::leaveEvent(QEvent* event) -> void {
    pimpl->leave_event(*this, *event);
    QAbstractButton::leaveEvent(event);
}

auto Switch::paintEvent(QPaintEvent* event) -> void { pimpl->paint_event(*this, *event); }
