#pragma once

#include <creeper-qt/utility/theme/theme.hh>
#include <creeper-qt/utility/wrapper/pimpl.hh>
#include <creeper-qt/widget/widget.hh>

struct AssetCenter : creeper::Widget {
    CREEPER_PIMPL_DEFINITION(AssetCenter)

public:
    explicit AssetCenter(creeper::ThemeManager& manager) noexcept;
};
