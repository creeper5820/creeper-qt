#pragma once
#include <creeper-qt/utility/theme/theme.hh>
#include <creeper-qt/widget/cards/filled-card.hh>

namespace creeper {

/// @function:
/// - 展示各种组件
/// - 抓取完整展示的图片
struct DisplayBoard : public FilledCard {
    explicit DisplayBoard(ThemeManager& manager);
};

}
