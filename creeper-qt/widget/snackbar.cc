#include "snackbar.hh"
#include "creeper-qt/utility/animation/state/pid.hh"
#include "creeper-qt/utility/animation/transition.hh"

using namespace creeper::snackbar::details;

struct Snackbar::Impl {
    Snackbar& self;

    struct Action {
        Message message;
        TransitionValue<PidState<double>> state;

        explicit Action() { }
    };
};

Snackbar::Snackbar()
    : pimpl { std::make_unique<Impl>(*this) } { }

Snackbar::~Snackbar() = default;
