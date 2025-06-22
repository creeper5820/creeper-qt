#pragma once

#include "creeper-qt/utility/wrapper/common-property.hh"
#include "creeper-qt/utility/wrapper/property.hh"

#include <qlayout.h>

namespace creeper::linear {

namespace pro {
    using Token = common::Token<QBoxLayout>;

    template <typename T>
    concept property_concept = std::derived_from<T, Token>;

    struct Item : Token {
        std::variant<QWidget*, QLayout*> item;
        int stretch         = 0;
        Qt::Alignment align = {};

        Item(QWidget* widget, int stretch = 0, Qt::Alignment align = {})
            : item(widget)
            , stretch(stretch)
            , align(align) { }

        Item(QLayout* layout, int stretch = 0)
            : item(layout)
            , stretch(stretch) { }

        void apply(QBoxLayout& self) const {
            std::visit(
                [&](auto* item) {
                    using ItemType = std::decay_t<decltype(item)>;
                    if constexpr (std::is_same_v<ItemType, QWidget*>) {
                        self.addWidget(item, stretch, align);
                    } else if constexpr (std::is_same_v<ItemType, QLayout*>) {
                        self.addLayout(item, stretch);
                    }
                },
                item);
        }
    };

    struct Items final : Token {
        std::vector<Item> items;

        explicit Items(std::vector<Item> args)
            : items { args } { }

        void apply(QBoxLayout& self) const {
            for (const auto& item : items)
                item.apply(self);
        }
    };

    struct Spacing final : Token {
        int size;
        explicit Spacing(int p) { size = p; }
        void apply(QBoxLayout& self) const { self.addSpacing(size); }
    };

    struct Stretch final : Token {
        int stretch;
        explicit Stretch(int p) { stretch = p; }
        void apply(QBoxLayout& self) const { self.addStretch(stretch); }
    };

    struct SpacerItem final : Token {
        QSpacerItem* spacer_item;
        explicit SpacerItem(QSpacerItem* p) { spacer_item = p; }
        void apply(QBoxLayout& self) const { self.addSpacerItem(spacer_item); }
    };

    // 属性类接口
    struct Margin final : Token {
        int margin;
        explicit Margin(int p) { margin = p; }
        void apply(QBoxLayout& self) const { self.setMargin(margin); }
    };
    struct SetSpacing final : Token {
        int size;
        explicit SetSpacing(int p) { size = p; }
        void apply(QBoxLayout& self) const { self.setSpacing(size); }
    };
    struct Alignment final : Token {
        Qt::Alignment alignment;
        explicit Alignment(Qt::Alignment p) { alignment = p; }
        void apply(QBoxLayout& self) const { self.setAlignment(alignment); }
    };

    struct ContentsMargin final : public QMargins, Token {
        using QMargins::QMargins;
        explicit ContentsMargin(int left, int top, int right, int bottom)
            : QMargins(left, top, right, bottom) { }
        void apply(QBoxLayout& self) const { self.setContentsMargins(*this); }
    };
}
}

namespace creeper {

class Row : public QHBoxLayout {
    CREEPER_DEFINE_CONSTROCTOR(Row, linear::pro)
    using QHBoxLayout::QHBoxLayout;
};

class Col : public QVBoxLayout {
    CREEPER_DEFINE_CONSTROCTOR(Col, linear::pro)
    using QVBoxLayout::QVBoxLayout;
};

}
