#pragma once

#include "creeper-qt/utility/wrapper/pimpl.hh"
#include <qwidget.h>

namespace creeper {

/// Animatable -> controller -> user
///                  |
/// Animatable <- updater
///
class Animatable {
    CREEPER_PIMPL_DEFINITION(Animatable)

public:
    template <typename T, class U>
    class Controller {
        std::shared_ptr<U> updater;
    };

public:
    explicit Animatable(QWidget& widget) noexcept;

    auto set_frame_rate(int hz) noexcept -> void;
    auto get_frame_rate() const noexcept -> int;

    template <class Controller>
    auto make_transition() noexcept -> std::unique_ptr<Controller>;
};

}
