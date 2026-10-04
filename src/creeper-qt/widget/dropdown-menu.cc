#include "dropdown-menu.impl.hh"

DropdownMenu::DropdownMenu()
    : QWidget(nullptr, Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint)
    , pimpl { std::make_unique<Impl>(*this) } { }

DropdownMenu::~DropdownMenu() = default;

auto DropdownMenu::loadColorScheme(const ColorScheme& scheme) -> void {
    pimpl->loadColorScheme(scheme);
}

auto DropdownMenu::bindThemeManager(ThemeManager& manager) -> void {
    pimpl->bindThemeManager(manager);
}

auto DropdownMenu::setAnchor(QWidget* widget) -> void { pimpl->set_anchor(widget); }

auto DropdownMenu::anchor() const noexcept -> QWidget* { return pimpl->anchor(); }

auto DropdownMenu::setExpanded(bool expanded) -> void { pimpl->set_expanded(expanded); }

auto DropdownMenu::expanded() const noexcept -> bool { return pimpl->expanded(); }

auto DropdownMenu::setOffset(QPoint offset) -> void { pimpl->set_offset(offset); }

auto DropdownMenu::setContainerColor(const QColor& color) -> void {
    pimpl->set_container_color(color);
}

auto DropdownMenu::setCornerRadius(double radius) -> void { pimpl->set_corner_radius(radius); }

auto DropdownMenu::addItem(QWidget* widget) -> void { pimpl->add_item(widget); }

auto DropdownMenu::contentCount() const noexcept -> int { return pimpl->content_count(); }

auto DropdownMenu::paintEvent(QPaintEvent* event) -> void { pimpl->paint_event(event); }

auto DropdownMenu::event(QEvent* event) -> bool {
    if (event->type() == QEvent::ParentChange) pimpl->parent_changed();
    return QWidget::event(event);
}

auto DropdownMenu::hideEvent(QHideEvent* event) -> void {
    pimpl->hide_event(event);
    QWidget::hideEvent(event);
}

auto DropdownMenu::eventFilter(QObject* watched, QEvent* event) -> bool {
    return pimpl->event_filter(watched, event);
}

auto DropdownMenu::wheelEvent(QWheelEvent* event) -> void { pimpl->wheel_event(event); }

auto DropdownMenu::keyPressEvent(QKeyEvent* event) -> void { pimpl->key_press_event(event); }
