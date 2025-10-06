#pragma once
#include <qmainwindow.h>

#include "creeper-qt/utility/trait/widget.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include "creeper-qt/utility/wrapper/property.hh"
#include "creeper-qt/utility/wrapper/widget.hh"

namespace creeper::main_window::internal {

template <class T>
concept central_widget_trait = requires(T t, QWidget* widget) {
    { t.setCentralWidget(widget) };
};

class MainWindow : public QMainWindow {
    CREEPER_PIMPL_DEFINITION(MainWindow)

protected:
    auto paintEvent(QPaintEvent*) -> void override;
};

}
namespace creeper::main_window::pro {
using Token = common::Token<QMainWindow>;

template <widget_trait T>
struct Central : Token {
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
concept trait = std::derived_from<T, Token>;

CREEPER_DEFINE_CHECKER(trait);
using namespace widget::pro;
}
namespace creeper {

using MainWindow = Declarative<main_window::internal::MainWindow,
    CheckerOr<main_window::pro::checker, widget::pro::checker>>;

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
