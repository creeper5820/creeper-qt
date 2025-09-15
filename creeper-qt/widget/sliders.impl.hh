#include "sliders.hh"
#include <qpainter.h>

using namespace creeper::slider::internal;

struct Slider::Impl {
public:
    explicit Impl(Slider& self) noexcept
        : self { self } { }

    auto paint_event(QPaintEvent*) -> void {
        auto painter = QPainter { &self };
        painter.setRenderHint(QPainter::Antialiasing);

        // Container
        { }
    }

    auto reverse_direction(bool reversed) -> void { }

    auto mouse_press_event(QMouseEvent* event) noexcept -> void { }

    auto mouse_release_event(QMouseEvent* event) noexcept -> void { }

    auto mouse_move_event(QMouseEvent* event) noexcept -> void {
        //
        emit self.signal_value_change(double {});
    }

private:
    auto update_progress(int spindle_position) noexcept -> void { }

private:
    double progress = 0.0;
    bool reversed   = false;

    Slider& self;
    ColorSpecs color_specs;
};
