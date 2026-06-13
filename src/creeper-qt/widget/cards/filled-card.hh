#pragma once
#include "basic-card.hh"

namespace creeper {
namespace filled_card::pro {
    using namespace card::pro;
}
using FilledCard = Declarative<card::internal::Card,
    TokenOr<card::pro::Token, rounded_rect::pro::Token, theme::pro::Token,
        widget::pro::Token>>;
}
