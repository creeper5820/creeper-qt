#pragma once
#include "basic-card.hh"

namespace creeper {
namespace filled_card::pro {
    template <class Card>
    concept property_concept = card::pro::property_concept<Card>;
}
class FilledCard : public card::internal::BasicCard {
    CREEPER_DEFINE_CONSTROCTOR(FilledCard, filled_card::pro);
    using card::internal::BasicCard::BasicCard;
};
}
