#pragma once
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/property.hh"
#include <qscrollarea.h>

namespace creeper::scroll::internal {

class ScrollArea : public QScrollArea { };

}
namespace creeper::scroll::pro {

using Token = common::Token<internal::ScrollArea>;

template <class T>
concept trait = std::derived_from<T, Token>;

CREEPER_DEFINE_CHECK(trait);
}
namespace creeper {

using ScrollArea = Declarative<scroll::internal::ScrollArea, scroll::pro::checker>;

}
