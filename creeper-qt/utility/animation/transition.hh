#pragma once

#include "animatable.hh"
#include "creeper-qt/utility/animation/math.hh"

#include <cmath>
#include <qdebug.h>

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

    auto transition_to(T to) noexcept -> void {
        // Update target of state
        state->set_target(std::move(to));

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

struct NormalAccessor {
    auto get_value(this auto const& self) { return self.value; }
    auto set_value(this auto& self, auto t) { self.value = t; }
    auto get_target(this auto const& self) { return self.target; }
    auto set_target(this auto& self, auto t) { self.target = t; }
};

template <typename T>
struct PidState : public NormalAccessor {
    using ValueT = T;

    using Clock     = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    T value  = animate::zero<T>();
    T target = animate::zero<T>();

    struct {
        double kp = 1.0;
        double ki = 0.0;
        double kd = 0.1;
    } config;

    struct {
        T integral_error = animate::zero<T>();
        T last_error     = animate::zero<T>();
        TimePoint last_timestamp;
    } details;

    static constexpr auto EPSILON = 1e-4;

    auto set_target(T new_target) noexcept -> void {
        target = new_target;

        const auto current_time = Clock::now();

        using namespace std::chrono_literals;
        const auto threshold = 16ms;

        const auto elapsed_time = current_time - details.last_timestamp;

        if (elapsed_time > threshold) {
            details.last_error     = target - value;
            details.last_timestamp = current_time;
        }
    }

    auto update() noexcept -> bool {

        const auto kp = config.kp;
        const auto ki = config.ki;
        const auto kd = config.kd;

        const auto now      = Clock::now();
        const auto duration = now - details.last_timestamp;

        const auto dt = std::chrono::duration<double>(duration).count();

        if (dt <= 0.0) {
            details.last_timestamp = now;
            return animate::magnitude(target - value) > EPSILON;
        }

        const auto current_error = target - value;

        if (animate::magnitude(current_error) <= EPSILON
            && animate::magnitude(details.last_error) <= EPSILON) {
            value                  = target;
            details.integral_error = animate::zero<T>();
            details.last_error     = animate::zero<T>();
            details.last_timestamp = now;
            return false;
        }

        const auto proportional_term = kp * current_error;

        details.integral_error += current_error * dt;
        const auto integral_term = ki * details.integral_error;

        const auto derivative_error = (current_error - details.last_error) / dt;
        const auto derivative_term  = kd * derivative_error;

        const auto output = proportional_term + integral_term + derivative_term;

        value += output * dt;

        details.last_error     = current_error;
        details.last_timestamp = now;

        return true;
    }
};

template <typename T>
struct SpringState : public NormalAccessor {
    using ValueT    = T;
    using Clock     = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    T value;
    T target;

    T velocity = animate::zero<T>();

    TimePoint last_timestamp = Clock::now();

    struct {
        double k               = 1.0;
        double d               = 0.1;
        double error_threshold = 1e-4;
    } config;

    auto set_target(T new_target) noexcept -> void {
        target = new_target;

        const auto current_time = Clock::now();

        using namespace std::chrono_literals;
        const auto threshold = 16ms;

        const auto elapsed_time = current_time - last_timestamp;

        if (elapsed_time > threshold) {
            const auto error = target - value;
            velocity         = animate::zero<T>();
            last_timestamp   = current_time;
        }
    }

    auto update() noexcept -> bool {
        const auto now      = Clock::now();
        const auto duration = now - last_timestamp;
        const double dt     = std::chrono::duration<double>(duration).count();

        if (dt <= 0.0) {
            last_timestamp = now;
            return std::abs(animate::magnitude(target - value)) > config.error_threshold;
        }

        const auto error     = value - target;
        const auto a_force   = -config.k * error;
        const auto a_damping = -config.d * velocity;
        const auto a_total   = a_force + a_damping;

        velocity += a_total * dt;
        value += velocity * dt;

        last_timestamp = now;

        const bool done = animate::magnitude(error) < config.error_threshold
            && std::abs(velocity) < config.error_threshold;
        return !done;
    }
};

}
