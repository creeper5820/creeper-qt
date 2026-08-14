#include "circular-progress-indicator.impl.hh"

CircularProgressIndicator::CircularProgressIndicator()
    : pimpl(std::make_unique<Impl>(*this)) { }

CircularProgressIndicator::~CircularProgressIndicator() = default;

void CircularProgressIndicator::set_color_scheme(const ColorScheme& scheme) {
    pimpl->set_color_scheme(*this, scheme);
}

void CircularProgressIndicator::load_theme_manager(ThemeManager& manager) {
    manager.append_handler(this, *this);
}

void CircularProgressIndicator::set_progress(double value) noexcept {
    pimpl->set_progress(*this, value);
}
double CircularProgressIndicator::progress() const noexcept { return pimpl->progress; }

void CircularProgressIndicator::set_indeterminate(bool on) noexcept {
    pimpl->set_indeterminate(*this, on);
}
bool CircularProgressIndicator::indeterminate() const noexcept { return pimpl->indeterminate; }

void CircularProgressIndicator::set_indicator_color(const QColor& color) noexcept {
    pimpl->indicator_color = color, update();
}
void CircularProgressIndicator::set_track_color(const QColor& color) noexcept {
    pimpl->track_color = color, update();
}
void CircularProgressIndicator::set_stroke_width(double width) noexcept {
    pimpl->stroke_width = width, update();
}

void CircularProgressIndicator::paintEvent(QPaintEvent* event) {
    pimpl->paint_event(*this, *event);
}
