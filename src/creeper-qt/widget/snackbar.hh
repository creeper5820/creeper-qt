#pragma once

#include "creeper-qt/details/token/snackbar/defaults.hh"
#include "creeper-qt/details/token/snackbar/measurements.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"

#include <qwidget.h>

namespace creeper {

enum class SnackbarResult { DISMISSED, ACTION_PERFORMED };

class Snackbar : public QWidget, DSL {
    CREEPER_PIMPL_DEFINITION(Snackbar);

public:
    using Measurements = token::snackbar::Measurements;
    using Colors       = token::snackbar::Colors;
    using Defaults     = token::snackbar::Defaults;

protected:
};

class SnackbarHost : public QWidget, DSL {
    CREEPER_PIMPL_DEFINITION(SnackbarHost);

public:
    struct State {
        CREEPER_PIMPL_DEFINITION(State);

    public:
        auto& showSnackbar();

    private:
        auto dismiss() noexcept -> void;
        auto perform() noexcept -> void;
    };

    explicit SnackbarHost(auto&&... props) noexcept
        : QWidget {} {
        construct_with(std::forward<decltype(props)>(props)...);
    }
};

}
namespace creeper::snackbar { }
namespace creeper::snackbar::host { }
