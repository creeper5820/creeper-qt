#include "document.hh"

using namespace document;
using namespace creeper;

struct NormalCard::Impl {
    FilledCard component;
};

NormalCard::NormalCard()
    : pimpl { std::make_unique<Impl>() } { }

NormalCard::~NormalCard() = default;

NormalCard::operator FilledCard&(this NormalCard && self) noexcept {
    //
    return self.pimpl->component;
}

auto NormalCard::set_brief(const QString&) noexcept -> void { }

auto NormalCard::add_param(const QString&) noexcept -> void { }

auto NormalCard::add_tparam(const QString&) noexcept -> void { }

auto NormalCard::add_note(const QString&) noexcept -> void { }
