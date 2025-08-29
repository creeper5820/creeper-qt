#pragma once

#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/property.hh"

namespace creeper::button::pro {

struct Button { };
using Token = common::Token<Button>;

template <class Button>
concept trait = std::derived_from<Button, Token>;

using Text        = common::pro::Text<Token>;
using TextColor   = common::pro::TextColor<Token>;
using Radius      = common::pro::Radius<Token>;
using BorderWidth = common::pro::BorderWidth<Token>;
using BorderColor = common::pro::BorderColor<Token>;
using Background  = common::pro::Background<Token>;
using WaterColor  = common::pro::WaterColor<Token>;

template <typename Callback>
using Clickable = common::pro::Clickable<Callback, Token>;

CREEPER_DEFINE_CHECK(trait)
}
