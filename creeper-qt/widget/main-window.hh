#pragma once
#include "creeper-qt/utility/trait/widget.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/property.hh"
#include "creeper-qt/widget/widget.hh"
#include <qmainwindow.h>

namespace creeper::main_window::internal {

template <class T>
concept central_widget_trait = requires(T t, QWidget* widget) {
    { t.setCentralWidget(widget) };
};

}
namespace creeper::main_window::pro {
using Token = common::Token<QMainWindow>;

template <widget_trait T> struct Central : Token {
    T* widget_pointer;

    explicit Central(T* pointer) noexcept
        : widget_pointer { pointer } { }

    explicit Central(auto&&... args) noexcept
        requires std::constructible_from<T, decltype(args)...>
        : widget_pointer {
            new T { std::forward<decltype(args)>(args)... },
        } { }
    auto apply(internal::central_widget_trait auto& self) const noexcept -> void {
        self.setCentralWidget(this->widget_pointer);
    }
};

template <class T>
concept trait = std::derived_from<T, Token> || widget::pro::trait<T>;

CREEPER_DEFINE_CHECK(trait);
using namespace widget::pro;
}
namespace creeper {

using MainWindow = Declarative<QMainWindow, main_window::pro::checker>;

/// @brief 一点显示窗口的语法糖
struct ShowWindow final {
    MainWindow* window_pointer;
    explicit ShowWindow(auto&&... args) noexcept
        requires std::constructible_from<MainWindow, decltype(args)...>
        : window_pointer {
            new MainWindow { std::forward<decltype(args)>(args)... },
        } {
        window_pointer->show();
    }
    explicit ShowWindow(MainWindow*& window, auto&&... args) noexcept
        requires std::constructible_from<MainWindow, decltype(args)...>
        : ShowWindow { std::forward<decltype(args)>(args)... } {
        window = window_pointer;
    }
    explicit ShowWindow(std::invocable<MainWindow&> auto f, auto&&... args) noexcept
        requires std::constructible_from<MainWindow, decltype(args)...>
        : ShowWindow { std::forward<decltype(args)>(args)... } {
        std::invoke(f, *window_pointer);
    }
};

}
