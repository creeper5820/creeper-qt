#pragma once
#include "animatable.hh"

namespace creeper {

template <class T>
concept transition_state_trait = requires(T& t) {
    typename T::ValueT;
    { t.get_value() } -> std::same_as<typename T::ValueT>;
    { t.get_target() } -> std::same_as<typename T::ValueT>;

    { t.set_value(std::declval<typename T::ValueT>()) };
    { t.set_target(std::declval<typename T::ValueT>()) };

    { t.update() } -> std::same_as<bool>;
};

// Functor like lambda
template <transition_state_trait State>
struct TransitionTask : public ITransitionTask {
public:
    explicit TransitionTask(std::shared_ptr<State> state, std::shared_ptr<bool> token) noexcept
        : state { std::move(state) }
        , running { std::move(token) } { }

    ~TransitionTask() override = default;

    auto update() noexcept -> bool override {
        return *running && state->update(); //
    }

private:
    std::shared_ptr<State> state;
    std::shared_ptr<bool> running;
};

template <transition_state_trait State>
struct TransitionValue {
public:
    using T = State::ValueT;

    explicit TransitionValue(Animatable& animatable, std::shared_ptr<State> state) noexcept
        : animatable { animatable }
        , state { std::move(state) } { }

    auto get_state() const noexcept -> const State& { return *state; }

    auto get_value() const noexcept { return state->get_value(); }

    auto get_target() const noexcept { return state->get_target(); }

    operator T() const noexcept { return state->get_value(); }

    auto transition_to(const T& to) noexcept -> void {
        // Update target of state
        state->set_target(to);

        // Clear last transition task
        if (running) {
            *running = false;
        }
        running = std::make_shared<bool>(true);

        // Push new transition task
        auto task = std::make_unique<TransitionTask<State>>(state, running);
        animatable.push_transition_task(std::move(task));
    }
    auto snap_to(T to) noexcept -> void {
        state->set_value(std::move(to));
        state->set_target(std::move(to));
        if (running) *running = false;
    }

private:
    std::shared_ptr<State> state;
    std::shared_ptr<bool> running;

    Animatable& animatable;
};

template <transition_state_trait State>
inline auto make_transition(Animatable& core, std::shared_ptr<State> state) {
    return std::make_unique<TransitionValue<State>>(core, state);
}

}
