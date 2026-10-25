#pragma once

#include "creeper-qt/utility/wrapper/dsl.hh"

#include <qwidget.h>

namespace creeper {

class Snakebar : public QWidget, DSL { };

class SnakebarHost : public QWidget, DSL {
public:
    struct State {
        auto showSnakebar();
    };

    explicit SnakebarHost(auto&&... props) noexcept
        : QWidget {} {
        construct_with(std::forward<decltype(props)>(props)...);
    }
};

}
