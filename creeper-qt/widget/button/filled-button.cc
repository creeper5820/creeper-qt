#include "filled-button.hh"

namespace creeper::internal {

struct FilledButton::Impl { };

FilledButton::FilledButton()
    : pimpl(std::make_unique<Impl>()) { }

FilledButton::~FilledButton() = default;

void FilledButton::set_radius(double radius) { }
void FilledButton::set_border(double border) { }

void FilledButton::set_water_color(const QColor& color) { }
void FilledButton::set_border_color(const QColor& color) { }
void FilledButton::set_text_color(const QColor& color) { }
void FilledButton::set_background_color(const QColor& color) { }

void FilledButton::set_water_ripple_status(bool enable) { }
void FilledButton::set_water_ripple_step(double step) { }

}

using namespace creeper;
