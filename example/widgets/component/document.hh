#pragma once
#include "creeper-qt/widget/cards/filled-card.hh"

namespace document {

class NormalCard final {
    CREEPER_PIMPL_DEFINITION(NormalCard);

public:
    operator creeper::FilledCard&(this NormalCard && self) noexcept;

    auto set_brief(const QString&) noexcept -> void;
    auto add_param(const QString&) noexcept -> void;
    auto add_tparam(const QString&) noexcept -> void;
    auto add_note(const QString&) noexcept -> void;
};

}
