#pragma once
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include "creeper-qt/utility/wrapper/property.hh"
#include "creeper-qt/utility/wrapper/widget.hh"

namespace creeper::lazy::details {

class LazyLayout : public QWidget {
    CREEPER_PIMPL_DEFINITION(LazyLayout)

public:
};

}
namespace creeper::lazy::pro {

using Token = creeper::Token<details::LazyLayout>;

using namespace widget::pro;

}
namespace creeper {

using LazyLayout =
    Declarative<lazy::details::LazyLayout, TokenOr<lazy::pro::Token, widget::pro::Token>>;
}
