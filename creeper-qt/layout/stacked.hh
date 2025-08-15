#pragma once
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/layout.hh"
#include "creeper-qt/utility/wrapper/property.hh"

#include <qstackedlayout.h>

namespace creeper::stacked::internal {
class Stacked : public QStackedLayout { };
}

namespace creeper::stacked::pro {

using Token = common::Token<internal::Stacked>;

template <typename T>
concept trait = std::derived_from<T, Token> || layout::pro::trait<T>;

CREEPER_DEFINE_CHECK(trait);
using namespace layout::pro;
}

namespace creeper {
using Stacked = Declarative<stacked::internal::Stacked, stacked::pro::checker>;
using NavHost = Stacked;
}
