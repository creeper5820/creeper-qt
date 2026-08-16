#pragma once
#include "creeper-qt/utility/wrapper/property.hh"
#include <vector>

namespace creeper::foreach::details {

// Generate With Data Only
template <std::ranges::range R, typename Generator>
    requires std::invocable<Generator, std::ranges::range_value_t<R>>
    && token_trait<std::invoke_result_t<Generator, std::ranges::range_value_t<R>>>
constexpr auto make_tokens(R&& range, Generator&& generator) {
    return std::forward<R>(range) | std::views::transform(std::forward<Generator>(generator))
        | std::ranges::to<std::vector>();
}

// Generate With Data And Index
template <std::ranges::range R, typename Generator>
    requires std::invocable<Generator, std::size_t, std::ranges::range_value_t<R>>
    && token_trait<std::invoke_result_t<Generator, std::size_t, std::ranges::range_value_t<R>>>
constexpr auto make_tokens(R&& range, Generator&& generator) {
    return std::forward<R>(range) | std::views::enumerate | std::views::transform([&](auto&& pair) {
        auto&& [index, value] = pair;
        return std::forward<Generator>(generator)(index, value);
    }) | std::ranges::to<std::vector>();
}

}

namespace creeper::Util {

template <std::ranges::range R, typename Generator>
constexpr auto ForEach(const R& range, Generator&& generator) {
    return foreach::details::make_tokens(range, std::forward<Generator>(generator));
}

}
