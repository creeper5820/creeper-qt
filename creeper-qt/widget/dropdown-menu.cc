#include "dropdown-menu.impl.hh"

DropdownMenu::DropdownMenu()
    : pimpl { std::make_unique<Impl>(*this) } { }

DropdownMenu::~DropdownMenu() = default;

void DropdownMenu::set_color_scheme(const ColorScheme& scheme) { pimpl->set_color_scheme(scheme); }

void DropdownMenu::load_theme_manager(ThemeManager& manager) { pimpl->load_theme_manager(manager); }

void DropdownMenu::set_label_text(const QString& text) { pimpl->set_label_text(text); }

void DropdownMenu::set_leading_icon(const QIcon&) { }

void DropdownMenu::set_leading_icon(const QString& code, const QString& font) {
    pimpl->set_leading_icon(code, font);
}

void DropdownMenu::resizeEvent(QResizeEvent* event) { QComboBox::resizeEvent(event); }

void DropdownMenu::enterEvent(qt::EnterEvent* enter_event) {
    pimpl->enter_event(enter_event);
    QComboBox::enterEvent(enter_event);
}

void DropdownMenu::leaveEvent(QEvent* event) {
    pimpl->leave_event(event);
    QComboBox::leaveEvent(event);
}

void DropdownMenu::focusInEvent(QFocusEvent* focus_event) {
    pimpl->focus_in(focus_event);
    QComboBox::focusInEvent(focus_event);
}

void DropdownMenu::focusOutEvent(QFocusEvent* event) {
    pimpl->focus_out(event);
    QComboBox::focusOutEvent(event);
}

void DropdownMenu::changeEvent(QEvent* event) { QComboBox::changeEvent(event); }

void DropdownMenu::showPopup() { pimpl->show_popup(); }

void DropdownMenu::hidePopup() { pimpl->hide_popup(); }

auto DropdownMenu::set_measurements(const Measurements& measurements) noexcept -> void {
    pimpl->set_measurements(measurements);
}

void DropdownMenu::setTextMargins(const QMargins& margins) { this->margins = margins; }

QMargins DropdownMenu::textMargins() const { return margins; }

using namespace creeper;

void FilledDropdownMenu::paintEvent(QPaintEvent* event) {
    pimpl->paint_filled(event);
    // QComboBox::paintEvent(event);
}