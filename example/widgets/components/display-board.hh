#pragma once
#include <creeper-qt/layout/linear.hh>
#include <creeper-qt/layout/stacked.hh>
#include <creeper-qt/utility/theme/theme.hh>
#include <creeper-qt/widget/cards/filled-card.hh>
#include <creeper-qt/widget/widget.hh>

namespace details::display_board {

using namespace creeper;
namespace wip = widget::pro;
namespace fcp = filled_card::pro;

struct DisplayWidget {
    QWidget* widget;
    std::string widget_name;
    std::string description;
};

/// @function:
/// - 展示各种组件
/// - 抓取完整展示的图片
struct DisplayBoard : public FilledCard {

    ThemeManager& manager;

    NavHost nav_host {
        nav_host::pro::CurrentIndex { 0 },
        nav_host::pro::Margin { 10 },
        nav_host::pro::Spacing { 10 },
    };
    std::unordered_map<const QWidget*, DisplayWidget> widget_map;

    auto Component() noexcept {
        return new Col {
            // ...
        };
    }

    auto append_widget(DisplayWidget widget) {
        nav_host.addWidget(widget.widget);
        widget_map.at(widget.widget) = std::move(widget);
    }

    explicit DisplayBoard(creeper::ThemeManager& manager)
        : FilledCard {
            fcp::ThemeManager { manager },
            fcp::Layout { Component() },
        }
        , manager{manager} { }
};

}

using DisplayBoard = details::display_board::DisplayBoard;
