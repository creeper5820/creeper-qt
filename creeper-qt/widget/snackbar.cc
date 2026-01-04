#include "snackbar.hh"
#include "creeper-qt/utility/animation/state/pid.hh"
#include "creeper-qt/utility/animation/transition.hh"

using namespace creeper::snackbar::details;

struct Snackbar::Impl {
    Snackbar& self;
    Animatable animatable { self };

    struct Action {
        Message message;
        TransitionValue<PidState<double>> state;

        explicit Action(Animatable& animatable)
            : state { animatable } { }
    };

    auto parent() const { return self.parentWidget(); }

    auto assembly() noexcept {
        if (parent() == nullptr) return;

        const auto& geometry = parent()->geometry();
    }
};

Snackbar::Snackbar()
    : pimpl { std::make_unique<Impl>(*this) } { }

Snackbar::~Snackbar() = default;
