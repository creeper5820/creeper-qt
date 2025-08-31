#pragma once

#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/layout.hh"
#include "creeper-qt/utility/wrapper/property.hh"

#include <qlayout.h>

namespace creeper::linear::pro {
using Token = common::Token<QBoxLayout>;

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
concept trait = std::derived_from<T, Token> || layout::pro::trait<T>;

CREEPER_DEFINE_CHECKER(trait);
using namespace layout::pro;
}

namespace creeper {

using Row = Declarative<QHBoxLayout, linear::pro::checker>;
using Col = Declarative<QVBoxLayout, linear::pro::checker>;

namespace row = linear;
namespace col = linear;

namespace internal {
    inline auto use_the_namespace_alias_to_eliminate_warnings() {
        std::ignore = row::pro::Token {};
        std::ignore = col::pro::Token {};
    }
}

}
