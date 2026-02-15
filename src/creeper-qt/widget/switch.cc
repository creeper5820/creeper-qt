#include "switch.impl.hh"

Switch::Switch()
    : pimpl(std::make_unique<Impl>(*this)) { }

Switch::~Switch() = default;

void Switch::set_color_scheme(const ColorScheme& scheme) {
    pimpl->set_color_scheme(*this, scheme), update();
}

void Switch::load_theme_manager(ThemeManager& manager) {
    manager.append_handler(
        this, [this](const ThemeManager& manager) { set_color_scheme(manager.color_scheme()); });
}

void Switch::set_disabled(bool on) { pimpl->set_disabled(*this, on); }
bool Switch::disabled() const { return pimpl->disabled; }

void Switch::set_checked(bool on) { pimpl->set_checked(*this, on); }
bool Switch::checked() const { return pimpl->checked; }

void Switch::set_track_color_unchecked(const QColor& color) { pimpl->track_unchecked = color; }
void Switch::set_track_color_checked(const QColor& color) { pimpl->track_checked = color; }
void Switch::set_track_color_unchecked_disabled(const QColor& color) {
    pimpl->track_unchecked_disabled = color;
}
void Switch::set_track_color_checked_disabled(const QColor& color) {
    pimpl->track_checked_disabled = color;
}

void Switch::set_handle_color_unchecked(const QColor& color) { pimpl->handle_unchecked = color; }
void Switch::set_handle_color_checked(const QColor& color) { pimpl->handle_checked = color; }
void Switch::set_handle_color_unchecked_disabled(const QColor& color) {
    pimpl->handle_unchecked_disabled = color;
}
void Switch::set_handle_color_checked_disabled(const QColor& color) {
    pimpl->handle_checked_disabled = color;
}

void Switch::set_outline_color_unchecked(const QColor& color) { pimpl->outline_unchecked = color; }
void Switch::set_outline_color_checked(const QColor& color) { pimpl->outline_checked = color; }
void Switch::set_outline_color_unchecked_disabled(const QColor& color) {
    pimpl->outline_unchecked_disabled = color;
}
void Switch::set_outline_color_checked_disabled(const QColor& color) {
    pimpl->outline_checked_disabled = color;
}

void Switch::set_hover_color_unchecked(const QColor& color) { pimpl->hover_unchecked = color; }
void Switch::set_hover_color_checked(const QColor& color) { pimpl->hover_checked = color; }

void Switch::enterEvent(qt::EnterEvent* event) {
    pimpl->enter_event(*this, *event);
    QAbstractButton::enterEvent(event);
}

void Switch::leaveEvent(QEvent* event) {
    pimpl->leave_event(*this, *event);
    QAbstractButton::leaveEvent(event);
}

void Switch::paintEvent(QPaintEvent* event) { pimpl->paint_event(*this, *event); }
