#pragma once

#include "creeper-qt/utility/wrapper/pimpl.hh"
#include <qwidget.h>

namespace creeper {

/// @note
/// Ends after the calculation is completed or the controller call ends
struct ITransitionTask {
    virtual ~ITransitionTask() noexcept    = default;
    virtual auto update() noexcept -> bool = 0;
};

class Animatable {
    CREEPER_PIMPL_DEFINITION(Animatable)

public:
    explicit Animatable(QWidget& widget) noexcept;

    auto set_frame_rate(int hz) noexcept -> void;
    auto get_frame_rate() const noexcept -> int;

    auto push_transition_task(std::unique_ptr<ITransitionTask> task) noexcept -> void;
};

}
