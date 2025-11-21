#pragma once
#include <creeper-qt/layout/linear.hh>
#include <creeper-qt/layout/stacked.hh>
#include <creeper-qt/utility/theme/theme.hh>
#include <creeper-qt/widget/widget.hh>

struct DisplayWidget {
    QWidget* widget;
    std::string widget_name;
    std::string description;
};

/// @function:
/// - 展示各种组件
/// - 抓取完整展示的图片
struct DisplayBoard : public creeper::Widget {

    creeper::ThemeManager& manager;

    creeper::NavHost nav_host {
        creeper::nav_host::pro::CurrentIndex { 0 },
        creeper::nav_host::pro::Margin { 10 },
        creeper::nav_host::pro::Spacing { 10 },
    };
    std::unordered_map<const QWidget*, DisplayWidget> widget_map;

    auto Component() noexcept {
        using namespace creeper;
        namespace wip = widget::pro;

        return wip::Layout<Col> {
            // ...
        };
    }

    auto append_widget(DisplayWidget widget) {
        nav_host.addWidget(widget.widget);
        widget_map.at(widget.widget) = std::move(widget);
    }

    explicit DisplayBoard(creeper::ThemeManager& manager)
        : manager { manager }
        , creeper::Widget { Component() } { }
};
