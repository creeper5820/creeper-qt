#include "animatable.hh"
using namespace creeper;

#include <qtimer.h>
using qwidget = QWidget;
using qtimer  = QTimer;

struct Animatable::Impl {

    qwidget& component;
    qtimer scheduler;

    int frame_rate = 90;

    explicit Impl(auto& component) noexcept
        : component { component } { }
};

Animatable::Animatable(QWidget& component) noexcept
    : pimpl { std::make_unique<Impl>(component) } { }

Animatable::~Animatable() = default;

auto Animatable::set_frame_rate(int hz) noexcept -> void {
    //
    pimpl->frame_rate = hz;
}
auto Animatable::get_frame_rate() const noexcept -> int {
    //
    return pimpl->frame_rate;
}
