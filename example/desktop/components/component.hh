/// 一次尝试，但这种写法不会为开发带来多少便捷性，特别是一些局部上下文的存储
/// 'display-board.hh' 这种写法可能更合适
#pragma once

#include <creeper-qt/utility/theme/theme.hh>
#include <qwidget.h>

template <typename T>
using raw_pointer = T*;

struct NavComponentState {
    creeper::ThemeManager& manager;
    std::function<void(int, const std::string_view&)> switch_callback;

    std::function<void()> next_tab = [] { qDebug() << "Unimplemented function"; };

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
