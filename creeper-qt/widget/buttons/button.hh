#pragma once

#include "creeper-qt/widget/widget.hh"
#include <qabstractbutton.h>

namespace creeper::button {
namespace pro {

    using Token = common::Token<QAbstractButton>;

    template <class Button>
    concept trait = std::derived_from<Button, Token> //
        || widget::pro::trait<Button>;

    using Text        = common::pro::Text<Token>;
    using TextColor   = common::pro::TextColor<Token>;
    using Radius      = common::pro::Radius<Token>;
    using BorderWidth = common::pro::BorderWidth<Token>;
    using BorderColor = common::pro::BorderColor<Token>;
    using Background  = common::pro::Background<Token>;
    using WaterColor  = common::pro::WaterColor<Token>;

    template <typename Callback> using Clickable = common::pro::Clickable<Callback, Token>;

    using namespace widget::pro;
}
}
