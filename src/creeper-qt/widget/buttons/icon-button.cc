#include "icon-button.impl.hh"

IconButton::IconButton()
    : pimpl(std::make_unique<Impl>(*this)) { }

IconButton::~IconButton() = default;

void IconButton::set_color_scheme(const ColorScheme& scheme) noexcept {
    pimpl->set_color_scheme(*this, scheme);
}
void IconButton::load_theme_manager(ThemeManager& manager) noexcept {
    pimpl->load_theme_manager(*this, manager);
}

void IconButton::enterEvent(qt::EnterEvent* event) {
    pimpl->enter_event(*this, *event);
    QAbstractButton::enterEvent(event);
}
void IconButton::leaveEvent(QEvent* event) {
    pimpl->leave_event(*this, *event);
    QAbstractButton::leaveEvent(event);
}

void IconButton::paintEvent(QPaintEvent* event) { pimpl->paint_event(*this, *event); }

void IconButton::set_icon(const QString& icon) noexcept { pimpl->font_icon = icon; }
void IconButton::set_icon(const QIcon& icon) noexcept { QAbstractButton::setIcon(icon); }

void IconButton::set_types(Types types) noexcept { pimpl->set_types_type(*this, types); }
void IconButton::set_shape(Shape shape) noexcept { pimpl->set_shape_type(*this, shape); }
void IconButton::set_color(Color color) noexcept { pimpl->set_color_type(*this, color); }
void IconButton::set_width(Width width) noexcept { pimpl->set_width_type(*this, width); }

auto IconButton::types_enum() const noexcept -> Types { return pimpl->types; }
auto IconButton::shape_enum() const noexcept -> Shape { return pimpl->shape; }
auto IconButton::color_enum() const noexcept -> Color { return pimpl->color; }
auto IconButton::width_enum() const noexcept -> Width { return pimpl->width; }

auto IconButton::selected() const noexcept -> bool {
    return pimpl->types == Types::TOGGLE_SELECTED;
}
auto IconButton::set_selected(bool selected) noexcept -> void {
    set_types(selected ? Types::TOGGLE_SELECTED : Types::TOGGLE_UNSELECTED);
};
