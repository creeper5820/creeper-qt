#pragma once

#include "utility/wrapper/property.hh"
#include "widget/widget.hh"
#include <qabstractbutton.h>

namespace creeper::button {
namespace pro {

    using Property = common::Token<QAbstractButton>;

    template <class Button>
    concept property_concept =
        std::derived_from<Button, Property> || widget::pro::property_concept<Button>;

    struct Clickable final : Property {
        std::function<void()> callback;
        explicit Clickable(std::function<void()> p) { callback = p; }
        void apply(auto& self) const
            requires requires { &std::remove_cvref_t<decltype(self)>::clicked; }
        {
            QObject::connect(&self, &QAbstractButton::clicked, [this] { callback(); });
        }
    };

    using Text        = common::pro::Text<Property>;
    using TextColor   = common::pro::TextColor<Property>;
    using Radius      = common::pro::Radius<Property>;
    using BorderWidth = common::pro::BorderWidth<Property>;
    using BorderColor = common::pro::BorderColor<Property>;
    using Background  = common::pro::Background<Property>;
    using WaterColor  = common::pro::WaterColor<Property>;

    using namespace widget::pro;
}
}
