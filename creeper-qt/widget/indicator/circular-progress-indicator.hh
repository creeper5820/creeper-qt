#pragma once
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include "creeper-qt/widget/widget.hh"

namespace creeper::circular_progress_indicator::internal {

class CircularProgressIndicator : public Widget {
    CREEPER_PIMPL_DEFINITION(CircularProgressIndicator);

public:
};

}
namespace creeper::circular_progress_indicator::pro {

using Token = common::Token<internal::CircularProgressIndicator>;

template <class T>
concept trait = std::derived_from<T, Token> || widget::pro::trait<T>;

CREEPER_DEFINE_CHECK(trait);
using namespace widget::pro;
}
namespace creeper { }
