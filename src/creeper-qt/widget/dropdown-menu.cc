#include "dropdown-menu.impl.hh"

DropdownMenu::DropdownMenu()
    : pimpl { std::make_unique<Impl>(*this) } { }

DropdownMenu::~DropdownMenu() = default;

auto DropdownMenu::set_color_scheme(const ColorScheme& scheme) -> void {
    pimpl->set_color_scheme(scheme);
}

auto DropdownMenu::load_theme_manager(ThemeManager& manager) -> void {
    pimpl->load_theme_manager(manager);
}

auto DropdownMenu::set_label_text(const QString& text) -> void { pimpl->set_label_text(text); }

auto DropdownMenu::set_leading_icon(const QIcon&) -> void { }

auto DropdownMenu::set_leading_icon(const QString& code, const QString& font) -> void {
    pimpl->set_leading_icon(code, font);
}

auto DropdownMenu::set_measurements(const Measurements& measurements) noexcept -> void {
    pimpl->set_measurements(measurements);
}

auto DropdownMenu::set_items(const QStringList& items) -> void { pimpl->set_items(items); }

auto DropdownMenu::set_current_index(int index) -> void { pimpl->set_current_index(index); }

auto DropdownMenu::current_index() const noexcept -> int { return pimpl->selected_index; }

auto DropdownMenu::current_text() const -> QString { return pimpl->current_text(); }

auto DropdownMenu::set_expanded(bool expanded) -> void { pimpl->set_expanded(expanded); }

auto DropdownMenu::expanded() const noexcept -> bool { return pimpl->expanded; }

auto DropdownMenu::enterEvent(qt::EnterEvent* event) -> void {
    pimpl->enter_event(event);
    QWidget::enterEvent(event);
}

auto DropdownMenu::leaveEvent(QEvent* event) -> void {
    pimpl->leave_event(event);
    QWidget::leaveEvent(event);
}

auto DropdownMenu::focusInEvent(QFocusEvent* event) -> void {
    pimpl->focus_in(event);
    QWidget::focusInEvent(event);
}

auto DropdownMenu::focusOutEvent(QFocusEvent* event) -> void {
    pimpl->focus_out(event);
    QWidget::focusOutEvent(event);
}

auto DropdownMenu::mousePressEvent(QMouseEvent* event) -> void {
    pimpl->mouse_press_event(event);
    QWidget::mousePressEvent(event);
}

auto DropdownMenu::keyPressEvent(QKeyEvent* event) -> void {
    pimpl->key_press_event(event);
    QWidget::keyPressEvent(event);
}

using namespace creeper;

auto FilledDropdownMenu::paintEvent(QPaintEvent* event) -> void { pimpl->paint_filled(event); }

auto OutlinedDropdownMenu::paintEvent(QPaintEvent* event) -> void {
    pimpl->paint_outlined(event);
}
