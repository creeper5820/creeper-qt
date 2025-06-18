#include "widget/switch.impl.hh"

Switch::Switch()
    : pimpl(std::make_unique<Impl>(*this)) { }

Switch::~Switch() = default;

void Switch::set_color_scheme(const ColorScheme& scheme) {
    pimpl->set_color_scheme(scheme), update();
}

void Switch::load_theme_manager(ThemeManager& manager) {
    manager.append_handler(this, [this](const ThemeManager& manager) {
        const auto color_mode   = manager.color_mode();
        const auto theme_pack   = manager.theme_pack();
        const auto color_scheme = color_mode == ColorMode::LIGHT //
            ? theme_pack.light
            : theme_pack.dark;
        set_color_scheme(color_scheme);
    });
}

void Switch::set_checked(bool on) { pimpl->checked = on; }
bool Switch::checked() const { return pimpl->checked; }

void Switch::set_track_color_unchecked(QColor color) { pimpl->track_unchecked = color; }
void Switch::set_track_color_checked(QColor color) { pimpl->track_checked = color; }
void Switch::set_track_color_disabled(QColor color) { pimpl->track_disabled = color; }

void Switch::set_handle_color_unchecked(QColor color) { pimpl->handle_unchecked = color; }
void Switch::set_handle_color_checked(QColor color) { pimpl->handle_checked = color; }
void Switch::set_handle_color_disabled(QColor color) { pimpl->handle_disabled = color; }

void Switch::set_outline_color_unchecked(QColor color) { pimpl->outline_unchecked = color; }
void Switch::set_outline_color_checked(QColor color) { pimpl->outline_checked = color; }
void Switch::set_outline_color_disabled(QColor color) { pimpl->outline_disabled = color; }

void Switch::enterEvent(QEvent* event) {
    pimpl->enter_event(*this, *event);
    QAbstractButton::enterEvent(event);
}

void Switch::leaveEvent(QEvent* event) {
    pimpl->leave_event(*this, *event);
    QAbstractButton::leaveEvent(event);
}

void Switch::paintEvent(QPaintEvent* event) { pimpl->paint_event(*this, *event); }
