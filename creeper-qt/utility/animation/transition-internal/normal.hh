#include "creeper-qt/utility/animation/transition.hh"
#include <atomic>
#include <concepts>

namespace creeper::animate {

static inline constexpr auto kEpsilon = 1;

template <typename T, typename F>
    requires std::invocable<F, T&>
struct Animate : public Transition::IAnimate {
public:
    explicit Animate(T&& start, F&& update) noexcept
        : t { std::forward<T>(start) }
        , f { std::forward<F>(update) }
        , stop_token { false } { }

    operator T(this auto&& self) noexcept { return self.t; }

    auto value() const noexcept -> T { return t; }

    auto stop() noexcept -> void { //
        stop_token.store(true, std::memory_order::relaxed);
    }
    auto stopped() const noexcept -> bool { //
        stop_token.load(std::memory_order::relaxed);
    }
    auto update() noexcept -> bool override {
        stop_token.store(f(), std::memory_order::relaxed);
        return stop_token.load(std::memory_order::relaxed);
    }

private:
    T t;
    F f;
    std::atomic<bool> stop_token;
};

template <typename T>
struct LinearCalculator {
    T step;
    T target;
    explicit LinearCalculator(T step, T target) noexcept
        : step { std::abs(step) }
        , target { target } { }
    auto operator()(T& value) noexcept {
        value += (value > target) ? -step : step;
        return (value - target) < 1e-3;
    }
};

using AnimateFloat  = Animate<float, LinearCalculator<float>>;
using AnimateDouble = Animate<double, LinearCalculator<double>>;
using AnimateLong   = Animate<long, LinearCalculator<long>>;

}
