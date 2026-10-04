#pragma once
#include "creeper-qt/utility/wrapper/dsl.hh"
#include <vector>

namespace creeper::Util {

// Generate With Data Only
template <std::ranges::range R, typename Generator>
    requires std::invocable<Generator, std::ranges::range_value_t<R>>
constexpr auto ForEach(const R& range, Generator&& generator) {
    return std::forward<decltype(range)>(range)
        | std::views::transform(std::forward<Generator>(generator))
        | std::ranges::to<std::vector>();
}

// Generate With Data And Index
template <std::ranges::range R, typename Generator>
    requires std::invocable<Generator, std::size_t, std::ranges::range_value_t<R>>
constexpr auto ForEach(const R& range, Generator&& generator) {
    return std::forward<decltype(range)>(range) | std::views::enumerate
        | std::views::transform([&](auto&& pair) {
              auto&& [index, value] = pair;
              return std::forward<Generator>(generator)(index, value);
          })
        | std::ranges::to<std::vector>();
}

}
