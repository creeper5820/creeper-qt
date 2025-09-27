#include "animatable.hh"
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

struct LinearFloatState : public NormalAccessor {
    using ValueT = float;

    float value;
    float target;

    auto update() noexcept -> bool {

        constexpr auto kStepSize = 0.01f;

        const auto diff = target - value;
        const auto step = std::copysign(kStepSize, diff);

        if (std::abs(diff) <= 0.03f) {
            value = target;
            return false;
        } else {
            value += step;
            return true;
        }
    }
};

template <typename T>
struct PidState : public NormalAccessor {
    using ValueT = T;

    using Clock     = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    T value;
    T target;

    struct {
        T kp = 1.0;
        T ki = 0.0;
        T kd = 0.1;
    } config;

    struct {
        T integral_error = 0.0;
        T last_error     = 0.0;
        TimePoint last_timestamp;
    } details;

    static constexpr T EPSILON = std::numeric_limits<T>::epsilon() * 1e5;

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

        const auto dt = std::chrono::duration<T>(duration).count();

        if (dt <= 0.0) {
            details.last_timestamp = now;
            return std::abs(target - value) > EPSILON;
        }

        const auto current_error = target - value;

        if (std::abs(current_error) <= EPSILON && std::abs(details.last_error) <= EPSILON) {
            value                  = target;
            details.integral_error = 0.0;
            details.last_error     = 0.0;
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

}
