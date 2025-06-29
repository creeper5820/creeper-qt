#include "icon-button.impl.hh"

IconButton::IconButton()
    : pimpl(std::make_unique<Impl>(*this)) { }

IconButton::~IconButton() = default;

void IconButton::set_color_scheme(const ColorScheme& scheme) {
    pimpl->set_color_scheme(*this, scheme);
}
void IconButton::load_theme_manager(ThemeManager& manager) {
    pimpl->load_theme_manager(*this, manager);
}

void IconButton::enterEvent(QEvent* event) {
    pimpl->enter_event(*this, *event);
    QAbstractButton::enterEvent(event);
}
void IconButton::leaveEvent(QEvent* event) {
    pimpl->leave_event(*this, *event);
    QAbstractButton::leaveEvent(event);
}

void IconButton::paintEvent(QPaintEvent* event) { pimpl->paint_event(*this, *event); }

void IconButton::set_icon(const QString& icon) { pimpl->font_icon = icon; }
void IconButton::set_icon(const QIcon& icon) { QAbstractButton::setIcon(icon); }

void IconButton::set_types(Types types) { pimpl->set_types_type(*this, types); }
void IconButton::set_shape(Shape shape) { pimpl->set_shape_type(*this, shape); }
void IconButton::set_color(Color color) { pimpl->set_color_type(*this, color); }
void IconButton::set_width(Width width) { pimpl->set_width_type(*this, width); }

auto IconButton::types() const -> Types { return pimpl->types; }
