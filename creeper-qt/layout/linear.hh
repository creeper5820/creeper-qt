#pragma once

#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/property.hh"

#include <qlayout.h>

namespace creeper::linear {

namespace pro {
    using Token = common::Token<QBoxLayout>;

    template <class T>
        requires std::is_convertible_v<T*, QWidget*> || std::is_convertible_v<T*, QLayout*>
    struct Item : Token {
        struct LayoutMethod {
            int stretch         = 0;
            Qt::Alignment align = {};
        } method;
        T* item_pointer = nullptr;

        explicit Item(const LayoutMethod& method, auto&&... args) noexcept
            requires std::constructible_from<T, decltype(args)...>
            : item_pointer { new T { std::forward<decltype(args)>(args)... } }
            , method(method) { }

        explicit Item(auto&&... args) noexcept
            requires std::constructible_from<T, decltype(args)...>
            : item_pointer { new T { std::forward<decltype(args)>(args)... } } { }

        explicit Item(const LayoutMethod& method, T* pointer) noexcept
            : item_pointer { pointer }
            , method { method } { }

        explicit Item(T* pointer) noexcept
            : item_pointer { pointer } { }

        void apply(QBoxLayout& layout) const {
            if constexpr (std::is_convertible_v<T*, QWidget*>)
                layout.addWidget(item_pointer, method.stretch, method.align);
            if constexpr (std::is_convertible_v<T*, QLayout*>)
                layout.addLayout(item_pointer, method.stretch);
        }
    };

    struct Spacing : Token {
        int size;
        explicit Spacing(int p) { size = p; }
        void apply(QBoxLayout& self) const { self.addSpacing(size); }
    };

    struct Stretch : Token {
        int stretch;
        explicit Stretch(int p) { stretch = p; }
        void apply(QBoxLayout& self) const { self.addStretch(stretch); }
    };

    struct SpacerItem : Token {
        QSpacerItem* spacer_item;
        explicit SpacerItem(QSpacerItem* p) { spacer_item = p; }
        void apply(QBoxLayout& self) const { self.addSpacerItem(spacer_item); }
    };

    // 属性类接口
    struct Margin : Token {
        int margin;
        explicit Margin(int p) { margin = p; }
        void apply(QBoxLayout& self) const { self.setMargin(margin); }
    };
    struct SetSpacing : Token {
        int size;
        explicit SetSpacing(int p) { size = p; }
        void apply(QBoxLayout& self) const { self.setSpacing(size); }
    };
    struct Alignment : Token {
        Qt::Alignment alignment;
        explicit Alignment(Qt::Alignment p) { alignment = p; }
        void apply(QBoxLayout& self) const { self.setAlignment(alignment); }
    };

    struct ContentsMargin : public QMargins, Token {
        using QMargins::QMargins;
        explicit ContentsMargin(int left, int top, int right, int bottom)
            : QMargins(left, top, right, bottom) { }
        void apply(QBoxLayout& self) const { self.setContentsMargins(*this); }
    };

    template <typename T>
    concept property_concept = std::derived_from<T, Token>;

    struct checker {
        template <class T> struct result {
            static constexpr auto v = property_concept<T>;
        };
    };
}
}

namespace creeper {

using Row = Declarative<QHBoxLayout, linear::pro::checker>;
using Col = Declarative<QVBoxLayout, linear::pro::checker>;

}
