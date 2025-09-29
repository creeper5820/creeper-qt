#include "sliders.impl.hh"

Slider::Slider()
    : pimpl { std::make_unique<Impl>(*this) } { }

Slider::~Slider() = default;

auto Slider::set_color_scheme(const ColorScheme& scheme) -> void {
    pimpl->set_color_scheme(scheme);
}
auto Slider::set_measurements(const Measurements& measurements) -> void {
    pimpl->set_measurements(measurements);
}
auto Slider::load_theme_manager(ThemeManager& manager) -> void {
    pimpl->load_theme_manager(manager);
}

auto Slider::set_progress(double progress) noexcept -> void {
    pimpl->set_progress(progress); //
}
auto Slider::get_progress() const noexcept -> double {
    return pimpl->get_progress(); //
}

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
