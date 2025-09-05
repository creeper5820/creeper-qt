#pragma once

#include <creeper-qt/utility/theme/theme.hh>
#include <creeper-qt/widget/cards/filled-card.hh>
#include <qdebug.h>

template <typename T>
using raw_pointer = T*;

constexpr auto default_switch_view_function = [](const std::string_view& name) {
    qDebug() << "[main] Switch to <" << name.data() << ">";
};

auto NavComponent(creeper::ThemeManager&,
    std::function<void(const std::string_view&)> switch_view_function =
        default_switch_view_function) noexcept -> raw_pointer<creeper::FilledCard>;

auto ListComponent(creeper::ThemeManager&) noexcept -> raw_pointer<creeper::FilledCard>;

auto ViewComponent(creeper::ThemeManager&) noexcept -> raw_pointer<creeper::FilledCard>;
