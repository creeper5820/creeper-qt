#pragma once
#include <cmath>
#include <generator>

namespace creeper {

struct LatticeSolution {
    double spacing { 0 };
    double x { 0 };
    double y { 0 };
    double w { 0 };
    double h { 0 };

    /// @note: order is w, h
    auto set_size(const auto& size) {
        auto [_w, _h]  = size;
        std::tie(w, h) = std::tie(_w, _h);
    }
    /// @note: order is x, y
    auto set_pos(const auto& point) {
        auto [_x, _y]  = point;
        std::tie(x, y) = std::tie(_x, _y);
    }

    auto solve() const noexcept -> std::generator<std::tuple<double, double>> {

        const auto count_x = static_cast<int>(std::floor(w / spacing));
        const auto count_y = static_cast<int>(std::floor(h / spacing));

        const auto views_x = std::views::iota(0, count_x + 1);
        const auto views_y = std::views::iota(0, count_y + 1);

        for (const auto [ix, iy] : std::views::cartesian_product(views_x, views_y)) {
            co_yield {
                std::lround(x + ix * spacing),
                std::lround(y + iy * spacing),
            };
        }
    }
};

}
