#pragma once

#include <creeper-qt/utility/theme/theme.hh>
#include <qwidget.h>

template <typename T>
using raw_pointer = T*;

struct NavComponentState {
    creeper::ThemeManager& manager;
    std::function<void(const std::string_view&)> switch_callback;
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
