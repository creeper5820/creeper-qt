#pragma once

#include <creeper-qt/utility/theme/theme.hh>
#include <creeper-qt/widget/widget.hh>

namespace creeper {

/// @brief Checkbox 状态矩阵展示项
struct CheckboxBoard : public Widget {
    explicit CheckboxBoard(ThemeManager& manager);
};

}
