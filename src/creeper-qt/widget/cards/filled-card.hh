#pragma once
#include "basic-card.hh"

namespace creeper {
namespace filled_card::pro {
    using namespace card::pro;
}
using FilledCard = Declarative<card::internal::Card,
    CheckerOr<card::pro::checker, rounded_rect::pro::checker, theme::pro::checker,
        widget::pro::checker>>;
}
