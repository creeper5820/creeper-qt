#include "snackbar.hh"

#include "creeper-qt/utility/co/meta/continuation.hh"
#include "creeper-qt/utility/wrapper/mutable-value.hh"

#include <qtimer.h>

namespace creeper {

struct Snackbar::Impl { };

struct SnackbarHost::State::Impl {
    co::continuation<SnackbarResult> continuation;
    QTimer timer;

    struct Data {
        // ...
    };
    MutableValue<Data> data;
};

struct SnackbarHost::Impl { };

}
