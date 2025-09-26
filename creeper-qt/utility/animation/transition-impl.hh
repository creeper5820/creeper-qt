#include "animatable.hh"
#include <cmath>

namespace creeper {

struct LinearFloatState {
    using T = float;

    float value;
    float target;

    auto update() noexcept -> bool {
        constexpr auto kStepSize = 0.05f;

        const auto diff = target - value;
        const auto step = std::copysign(kStepSize, diff);

        if (std::abs(diff) <= std::abs(step)) {
            value = target;
            return true;
        } else {
            value += step;
            return true;
        }
    }
};

template <class State>
inline auto make_transition(Animatable& core, std::shared_ptr<State> state) {

    struct TransitionTask : ITransitionTask {
    public:
        explicit TransitionTask(std::shared_ptr<State> state, std::shared_ptr<bool> token) noexcept
            : pimpl { std::move(state) }
            , running { std::move(token) } { }

        ~TransitionTask() override = default;

        auto update() noexcept -> bool override { return pimpl->update(); }

    private:
        std::shared_ptr<State> pimpl;
        std::shared_ptr<bool> running;
    };

    struct Handler {
    public:
        using T = State::T;

        explicit Handler(Animatable& animatable, std::shared_ptr<State> state) noexcept
            : animatable { animatable }
            , state { std::move(state) } { }

        operator T() const noexcept { return state.value; }

        auto transition_to(T to) noexcept -> void {
            // Update target of state
            state->target = to;

            // Clear last transition task
            if (running.has_value()) {
                *running.value() = false;
            }
            running = std::make_shared<bool>(true);

            // Push new transition task
            auto task = std::make_unique<TransitionTask>(state, running);
            animatable.push_transition_task(std::move(task));
        }

    private:
        std::shared_ptr<State> state;
        std::optional<std::shared_ptr<bool>> running;

        Animatable& animatable;
    };

    return std::make_unique<Handler>(core, state);
}

}
