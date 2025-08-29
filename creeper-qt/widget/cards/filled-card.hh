#pragma once
#include "basic-card.hh"

namespace creeper {
namespace filled_card::pro {
    using namespace card::pro;
}
using FilledCard = Declarative<BasicCard, BasicCard::Checker>;
}
