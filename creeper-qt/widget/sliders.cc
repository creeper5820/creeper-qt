#include "sliders.impl.hh"

Slider::Slider()
    : pimpl { std::make_unique<Impl>(*this) } { }

Slider::~Slider() = default;

auto Slider::mousePressEvent(QMouseEvent* event) -> void {
    pimpl->mouse_press_event(event);
    Widget::mousePressEvent(event);
}
auto Slider::mouseReleaseEvent(QMouseEvent* event) -> void {
    pimpl->mouse_release_event(event);
    Widget::mouseReleaseEvent(event);
}
auto Slider::mouseMoveEvent(QMouseEvent* event) -> void {
    pimpl->mouse_move_event(event);
    Widget::mouseMoveEvent(event);
}
auto Slider::paintEvent(QPaintEvent* event) -> void { pimpl->paint_event(event); }
