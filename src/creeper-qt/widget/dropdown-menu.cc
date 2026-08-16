#include "dropdown-menu.impl.hh"

DropdownMenu::DropdownMenu()
    : QWidget(nullptr, Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint)
    , pimpl { std::make_unique<Impl>(*this) } { }

DropdownMenu::~DropdownMenu() = default;

auto DropdownMenu::set_color_scheme(const ColorScheme& scheme) -> void {
    pimpl->set_color_scheme(scheme);
}

auto DropdownMenu::load_theme_manager(ThemeManager& manager) -> void {
    pimpl->load_theme_manager(manager);
}

auto DropdownMenu::set_anchor(QWidget* widget) -> void { pimpl->set_anchor(widget); }

auto DropdownMenu::anchor() const noexcept -> QWidget* { return pimpl->anchor(); }

auto DropdownMenu::set_expanded(bool expanded) -> void { pimpl->set_expanded(expanded); }

auto DropdownMenu::expanded() const noexcept -> bool { return pimpl->expanded(); }

auto DropdownMenu::set_offset(QPoint offset) -> void { pimpl->set_offset(offset); }

auto DropdownMenu::set_container_color(const QColor& color) -> void {
    pimpl->set_container_color(color);
}

auto DropdownMenu::set_corner_radius(double radius) -> void { pimpl->set_corner_radius(radius); }

auto DropdownMenu::add_item(QWidget* widget) -> void { pimpl->add_item(widget); }

auto DropdownMenu::content_count() const noexcept -> int { return pimpl->content_count(); }

auto DropdownMenu::paintEvent(QPaintEvent* event) -> void { pimpl->paint_event(event); }

auto DropdownMenu::hideEvent(QHideEvent* event) -> void {
    pimpl->hide_event(event);
    QWidget::hideEvent(event);
}

auto DropdownMenu::eventFilter(QObject* watched, QEvent* event) -> bool {
    return pimpl->event_filter(watched, event);
}

auto DropdownMenu::wheelEvent(QWheelEvent* event) -> void { pimpl->wheel_event(event); }

auto DropdownMenu::keyPressEvent(QKeyEvent* event) -> void { pimpl->key_press_event(event); }
