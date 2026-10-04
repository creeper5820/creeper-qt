#include "circular-progress-indicator.impl.hh"

CircularProgressIndicator::CircularProgressIndicator()
    : pimpl(std::make_unique<Impl>(*this)) { }

CircularProgressIndicator::~CircularProgressIndicator() = default;

auto CircularProgressIndicator::loadColorScheme(const ColorScheme& scheme) -> void {
    pimpl->loadColorScheme(*this, scheme);
}

auto CircularProgressIndicator::bindThemeManager(ThemeManager& manager) -> void {
    manager.appendHandler(this, *this);
}

auto CircularProgressIndicator::setProgress(double value) noexcept -> void {
    pimpl->set_progress(*this, value);
}
auto CircularProgressIndicator::progress() const noexcept -> double { return pimpl->progress; }

auto CircularProgressIndicator::setIndeterminate(bool on) noexcept -> void {
    pimpl->set_indeterminate(*this, on);
}
auto CircularProgressIndicator::indeterminate() const noexcept -> bool {
    return pimpl->indeterminate;
}

auto CircularProgressIndicator::setIndicatorColor(const QColor& color) noexcept -> void {
    pimpl->indicator_color = color, update();
}
auto CircularProgressIndicator::setTrackColor(const QColor& color) noexcept -> void {
    pimpl->track_color = color, update();
}
auto CircularProgressIndicator::setStrokeWidth(double width) noexcept -> void {
    pimpl->stroke_width = width, update();
}

auto CircularProgressIndicator::paintEvent(QPaintEvent* event) -> void {
    pimpl->paint_event(*this, *event);
}
