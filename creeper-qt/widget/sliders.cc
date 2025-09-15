#include "sliders.impl.hh"

Slider::Slider()
    : pimpl { std::make_unique<Impl>(*this) } { }

Slider::~Slider() = default;

auto Slider::mousePressEvent(QMouseEvent* event) -> void {
    //
    Widget::mousePressEvent(event);
}
auto Slider::mouseReleaseEvent(QMouseEvent* event) -> void {
    //
    Widget::mouseReleaseEvent(event);
}
auto Slider::mouseMoveEvent(QMouseEvent* event) -> void {
    //
    Widget::mouseMoveEvent(event);
}
auto Slider::paintEvent(QPaintEvent* event) -> void { pimpl->paint_event(event); }
