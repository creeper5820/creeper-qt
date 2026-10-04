#include "icon-button.impl.hh"

IconButton::IconButton()
    : pimpl(std::make_unique<Impl>(*this)) { }

IconButton::~IconButton() = default;

void IconButton::loadColorScheme(const ColorScheme& scheme) noexcept {
    pimpl->loadColorScheme(*this, scheme);
}
void IconButton::bindThemeManager(ThemeManager& manager) noexcept {
    pimpl->bindThemeManager(*this, manager);
}

void IconButton::resizeEvent(QResizeEvent* event) {
    pimpl->resize_event(*this, *event);
    QAbstractButton::resizeEvent(event);
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

void IconButton::setFontIcon(const QString& icon) noexcept { pimpl->font_icon = icon; }
void IconButton::setIcon(const QIcon& icon) noexcept { QAbstractButton::setIcon(icon); }

void IconButton::setTypes(Types types) noexcept { pimpl->set_types_type(*this, types); }
void IconButton::setShape(Shape shape) noexcept { pimpl->set_shape_type(*this, shape); }
void IconButton::setColor(Color color) noexcept { pimpl->set_color_type(*this, color); }
void IconButton::setWidth(Width width) noexcept { pimpl->set_width_type(*this, width); }

auto IconButton::typesEnum() const noexcept -> Types { return pimpl->types; }
auto IconButton::shapeEnum() const noexcept -> Shape { return pimpl->shape; }
auto IconButton::colorEnum() const noexcept -> Color { return pimpl->color; }
auto IconButton::widthEnum() const noexcept -> Width { return pimpl->width; }

auto IconButton::selected() const noexcept -> bool {
    return pimpl->types == Types::TOGGLE_SELECTED;
}
auto IconButton::setSelected(bool selected) noexcept -> void {
    setTypes(selected ? Types::TOGGLE_SELECTED : Types::TOGGLE_UNSELECTED);
}
