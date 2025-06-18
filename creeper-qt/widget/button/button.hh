#pragma once

#include "widget/widget.hh"
#include <qabstractbutton.h>

namespace creeper::button {
namespace pro {

    using Token = common::Token<QAbstractButton>;

    template <class Button>
    concept property_concept = std::derived_from<Button, Token> //
        || widget::pro::property_concept<Button>;

    struct Clickable final : Token {
        std::function<void()> callback;
        explicit Clickable(const std::function<void()>& p) { callback = p; }
        void apply(auto& self) const
            requires requires { &std::remove_cvref_t<decltype(self)>::clicked; }
        {
            QObject::connect(&self, &QAbstractButton::clicked, [this] { callback(); });
        }
    };

    using Text        = common::pro::Text<Token>;
    using TextColor   = common::pro::TextColor<Token>;
    using Radius      = common::pro::Radius<Token>;
    using BorderWidth = common::pro::BorderWidth<Token>;
    using BorderColor = common::pro::BorderColor<Token>;
    using Background  = common::pro::Background<Token>;
    using WaterColor  = common::pro::WaterColor<Token>;

    using namespace widget::pro;
}
}
