#pragma once

#include <creeper-qt/utility/theme/theme.hh>
#include <creeper-qt/widget/widget.hh>

namespace creeper {

/// @brief Switch 状态矩阵展示项
struct SwitchBoard : public Widget {
    explicit SwitchBoard(ThemeManager& manager);
};

}
