#pragma once
#include "creeper-qt/utility/trait/widget.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include "creeper-qt/utility/wrapper/property.hh"
#include "creeper-qt/utility/wrapper/widget.hh"

namespace creeper::lazy::details {

class LazyLayout : public QWidget {
    CREEPER_PIMPL_DEFINITION(LazyLayout)

public:
};

class LazyColumn : public LazyLayout {
public:
    LazyColumn() {
        // ......
    }
};
class LazyRow : public LazyLayout {
public:
    LazyRow() {
        // ......
    }
};

}
namespace creeper::lazy::pro {

using Token = creeper::Token<details::LazyLayout>;

template <widget_trait T>
struct Item : Token { };

template <widget_trait T>
struct Items : Token {

    template <std::ranges::range Range>
    explicit Items(Range range) { }
};

using namespace widget::pro;
}
namespace creeper {

using LazyLayout =
    Declarative<lazy::details::LazyLayout, TokenOr<lazy::pro::Token, widget::pro::Token>>;
}
