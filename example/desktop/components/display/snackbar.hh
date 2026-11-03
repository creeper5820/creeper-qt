#pragma once

#include <creeper-qt/utility/theme/theme.hh>
#include <creeper-qt/widget/widget.hh>

namespace creeper {

/// @brief Snackbar 变体与交互状态展示项
struct SnackbarBoard : public Widget {
    explicit SnackbarBoard(ThemeManager& manager);
};

}
