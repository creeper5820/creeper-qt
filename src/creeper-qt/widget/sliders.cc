#include "sliders.impl.hh"

Slider::Slider()
    : pimpl { std::make_unique<Impl>(*this) } { }

Slider::~Slider() = default;

auto Slider::loadColorScheme(const ColorScheme& scheme) -> void { pimpl->loadColorScheme(scheme); }
auto Slider::setMeasurements(const Measurements& measurements) -> void {
    pimpl->set_measurements(measurements);
}
auto Slider::bindThemeManager(ThemeManager& manager) -> void { pimpl->bindThemeManager(manager); }

auto Slider::setProgress(double progress) noexcept -> void {
    pimpl->set_progress(progress); //
}
auto Slider::getProgress() const noexcept -> double {
    return pimpl->get_progress(); //
}

auto Slider::mousePressEvent(QMouseEvent* event) -> void {
    pimpl->mouse_press_event(event);
    QWidget::mousePressEvent(event);
}
auto Slider::mouseReleaseEvent(QMouseEvent* event) -> void {
    pimpl->mouse_release_event(event);
    QWidget::mouseReleaseEvent(event);
}
auto Slider::mouseMoveEvent(QMouseEvent* event) -> void {
    pimpl->mouse_move_event(event);
    QWidget::mouseMoveEvent(event);
}
auto Slider::paintEvent(QPaintEvent* event) -> void { pimpl->paint_event(event); }
