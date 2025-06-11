#pragma once

#include <qabstractbutton.h>

namespace creeper {

namespace pro::button {

    template <std::derived_from<QAbstractButton> Widget, class Token>
    struct Clickable final : Token {
        std::function<void()> callback;
        explicit Clickable(std::function<void()> p) { callback = p; }
        void apply(Widget& self) const override {
            QObject::connect(&self, &QAbstractButton::clicked, [this] { callback(); });
        }
    };

}

}
