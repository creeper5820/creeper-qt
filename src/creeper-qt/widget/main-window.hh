#pragma once

#include "creeper-qt/utility/api/scope/common.hh" // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/widget.hh" // IWYU pragma: keep
#include "creeper-qt/utility/trait/widget.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/forward_prop.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"

#include <qmainwindow.h>

namespace creeper {

class MainWindow : public QMainWindow, public DSL {
    CREEPER_PIMPL_DEFINITION(MainWindow)

public:
    using QMainWindow::QMainWindow;

    explicit MainWindow(auto&&... props)
        : MainWindow { } {
        construct_with(std::forward<decltype(props)>(props)...);
    }
};

}
namespace creeper::main_window::pro {
template <widget_trait T>
struct Central {
    T* widget_pointer;

    explicit Central(T* pointer) noexcept
        : widget_pointer { pointer } { }

    explicit Central(auto&&... args) noexcept
        requires std::constructible_from<T, decltype(args)...>
        : widget_pointer {
            new T { std::forward<decltype(args)>(args)... },
        } { }
    friend auto dsl_invoke(MainWindow& self, const Central& prop) -> void {
        self.setCentralWidget(prop.widget_pointer);
    }
};

using namespace api::scope::common;
using namespace api::scope::widget;
}
namespace creeper {

/// @brief 一点显示窗口的语法糖
template <widget_trait T>
struct ShowWindow final {
    T* window_pointer;
    explicit ShowWindow(auto&&... args) noexcept
        requires std::constructible_from<T, decltype(args)...>
        : window_pointer {
            new T { std::forward<decltype(args)>(args)... },
        } {
        window_pointer->show();
    }
    explicit ShowWindow(T*& window, auto&&... args) noexcept
        requires std::constructible_from<T, decltype(args)...>
        : ShowWindow { std::forward<decltype(args)>(args)... } {
        window = window_pointer;
    }
    explicit ShowWindow(std::invocable<T&> auto f, auto&&... args) noexcept
        requires std::constructible_from<T, decltype(args)...>
        : ShowWindow { std::forward<decltype(args)>(args)... } {
        std::invoke(f, *window_pointer);
    }
};

}
