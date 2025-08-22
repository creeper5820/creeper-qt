#include "transition.hh"
#include <algorithm>
#include <qtimer.h>

using namespace creeper::animate;

struct Transition::Impl {
    QTimer schedular;
    std::vector<std::shared_ptr<IAnimate>> animates;

    explicit Impl() noexcept {
        schedular.connect(&schedular, &QTimer::timeout, [this] { update(); });
    }

    auto update() noexcept -> void {
        const auto [begin, end] = std::ranges::remove_if(
            animates, [](const std::shared_ptr<IAnimate>& animate) { return animate->update(); });

        animates.erase(begin, end);
    }

    template <std::derived_from<IAnimate> T, typename... Args>
        requires std::constructible_from<T, Args...>
    auto emplace_back(Args&&... args) noexcept {
        auto animate_pointer = std::make_shared<T>(std::forward<Args>(args)...);
        animates.push_back(std::static_pointer_cast<IAnimate>(animate_pointer));
        return animate_pointer;
    }
};

Transition::Transition()
    : pimpl { std::make_unique<Impl>() } { }

Transition::~Transition() = default;
