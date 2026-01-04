#pragma once

#include <qwidget.h>

namespace creeper {

/// @note
/// Ends after the calculation is completed or the controller call ends
struct ITransitionTask {
    virtual ~ITransitionTask() noexcept    = default;
    virtual auto update() noexcept -> bool = 0;
};

class Animatable {
public:
    ~Animatable();
    Animatable(const Animatable&)            = delete;
    Animatable& operator=(const Animatable&) = delete;

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl;

public:
    explicit Animatable(QWidget& widget) noexcept;

    auto set_frame_rate(int hz) noexcept -> void;
    auto get_frame_rate() const noexcept -> int;

    auto push_transition_task(std::unique_ptr<ITransitionTask> task) noexcept -> void;
};

}
