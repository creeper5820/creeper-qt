#include "scroll.hh"
#include "creeper-qt/utility/animation/state/pid.hh"
#include "creeper-qt/utility/animation/transition.hh"

using namespace creeper::scrollable::details;

struct Scrollable::Impl {
    Scrollable& self;

    Animatable animatable { self };
    TransitionValue<PidState<Eigen::Vector2d>> scroll_state { animatable };

    QWidget* viewport = new QWidget { &self };

    explicit Impl(Scrollable& self)
        : self { self } { }

    auto set_layout(QLayout* layout) noexcept { viewport->setLayout(layout); }

    auto snap_to(Eigen::Vector2d to) { scroll_state.snap_to(to); }

    auto transition_to(Eigen::Vector2d to) { scroll_state.transition_to(to); }

    auto update_viewport() noexcept {
        viewport->move(scroll_state.get_value().x(), scroll_state.get_value().y());
    }
};

auto Scrollable::paintEvent(QPaintEvent*) -> void { pimpl->update_viewport(); }

Scrollable::Scrollable()
    : pimpl { std::make_unique<Impl>(*this) } { }

Scrollable::~Scrollable() = default;
