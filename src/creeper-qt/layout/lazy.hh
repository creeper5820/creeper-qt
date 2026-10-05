#pragma once

#include "creeper-qt/utility/api/scope/common.hh" // IWYU pragma: keep

// TODO: 尚未实现。LazyColumn/LazyRow 构造函数与 Item/Items 属性均为占位，
//       惰性布局逻辑待补全。
#include "creeper-qt/utility/api/scope/widget.hh" // IWYU pragma: keep
#include "creeper-qt/utility/trait/widget.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"

namespace creeper {

class LazyLayout : public QWidget, public DSL {
    CREEPER_PIMPL_DEFINITION(LazyLayout)

public:
    explicit LazyLayout(auto&&... props)
        : LazyLayout { } {
        construct_with(std::forward<decltype(props)>(props)...);
    }
};

class LazyColumn : public LazyLayout {
public:
    explicit LazyColumn(auto&&... props)
        : LazyLayout { } {
        construct_with(std::forward<decltype(props)>(props)...);
    }
};

class LazyRow : public LazyLayout {
public:
    explicit LazyRow(auto&&... props)
        : LazyLayout { } {
        construct_with(std::forward<decltype(props)>(props)...);
    }
};

}
namespace creeper::lazy::pro {
/// @note 占位属性，惰性布局逻辑待补全
template <widget_trait T>
struct Item {
    friend auto dsl_invoke(auto& self, const Item&) -> void { }
};
/// @note 占位属性，惰性布局逻辑待补全
template <widget_trait T>
struct Items {
    template <std::ranges::range Range>
    explicit Items(Range) { }

    friend auto dsl_invoke(auto& self, const Items&) -> void { }
};

using namespace api::scope::common;
using namespace api::scope::widget;
}
