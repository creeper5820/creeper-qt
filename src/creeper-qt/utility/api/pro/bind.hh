#pragma once

#include <type_traits>

namespace creeper::api::pro {

/// @brief
/// 绑定裸指针，为了避免赋值语句
///
/// 一般情况下需要绑定指针：
///
/// 假设下面是一个 layout 的内部
/// clang-format 会在下面的赋值符号后换行
/// @code
///     auto widget = (Widget*)nullptr;
///     ......
///     { widget =
///             new Widget {
///                 ......
///             } },
///     ......
/// @endcode
///
/// 利用赋值语句的返回特性将该组件返回给 layout，同时完成赋值，
/// 但这样会多一层缩进，为保证构造配置的简洁和同一，可以使用该包装：
///
/// @code
///     ......
///     { new Widget {
///         pro::Bind { widget },
///     } },
///     ......
/// @endcode
///
/// @tparam Final 需要绑定的组件类型（自动推导，无需显式指定）
///
/// @date 2025-06-19
template <class Final>
struct Bind {
    Final*& widget;

    explicit Bind(Final*& widget) noexcept
        requires std::is_pointer_v<Final*>
        : widget(widget) { }

    friend auto dsl_invoke(Final& self, const Bind& prop) -> void { prop.widget = &self; }
};

}
