#pragma once

#include <creeper-qt/utility/theme/theme.hh>
#include <qwidget.h>

template <typename T>
using raw_pointer = T*;

struct NavComponentState {
    creeper::ThemeManager& manager;
    std::function<void(int, const std::string_view&)> switch_callback;

    std::vector<std::tuple<std::string_view, std::string_view>> buttons_context;
};
auto NavComponent(NavComponentState&) noexcept -> raw_pointer<QWidget>;

struct ListComponentState {
    creeper::ThemeManager& manager;
};
auto ListComponent(ListComponentState&) noexcept -> raw_pointer<QWidget>;

struct ViewComponentState {
    creeper::ThemeManager& manager;
};
auto ViewComponent(ViewComponentState&) noexcept -> raw_pointer<QWidget>;
