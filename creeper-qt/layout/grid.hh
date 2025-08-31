#pragma once

#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/property.hh"

#include <qgridlayout.h>

namespace creeper {
namespace grid::internal {
    class Grid : public QGridLayout { };
}
namespace grid::pro {
    using Token = common::Token<QGridLayout>;

    struct RowSpacing : Token { };

    struct ColSpacing : Token { };

    template <class T>
        requires std::is_convertible_v<T*, QWidget*> || std::is_convertible_v<T*, QLayout*>
    struct Item : Token {
        using Align = Qt::Alignment;

        struct LayoutMethod {
            int row = 0, row_span = 0;
            int col = 0, col_span = 0;
            Align align;

            explicit LayoutMethod(int row, int col, Align align = {})
                : row { row }
                , col { col }
                , align { align } { }

            explicit LayoutMethod(int row, int row_span, int col, int col_span, Align align = {})
                : row { row }
                , col { col }
                , row_span { row_span }
                , col_span { col_span }
                , align { align } { }

        } method;

        T* item_pointer = nullptr;

        explicit Item(const LayoutMethod& method, auto&&... args) noexcept
            requires std::constructible_from<T, decltype(args)...>
            : item_pointer { new T { std::forward<decltype(args)>(args)... } }
            , method(method) { }

        explicit Item(const LayoutMethod& method, T* pointer) noexcept
            : item_pointer { pointer }
            , method { method } { }

        void apply(QGridLayout& layout) const {
            if (method.col_span == 0) {
                if constexpr (std::is_convertible_v<T*, QWidget*>)
                    layout.addWidget(item_pointer, method.row, method.col, method.align);
                if constexpr (std::is_convertible_v<T*, QLayout*>)
                    layout.addLayout(item_pointer, method.row, method.col, method.align);
            } else {
                if constexpr (std::is_convertible_v<T*, QWidget*>)
                    layout.addWidget(item_pointer, method.row, method.row_span, method.col,
                        method.col_span, method.align);
                if constexpr (std::is_convertible_v<T*, QLayout*>)
                    layout.addLayout(item_pointer, method.row, method.row_span, method.col,
                        method.col_span, method.align);
            }
        }
    };

    struct Items : Token {
        explicit Items() { }
        void apply(QGridLayout& self) const { }
    };

    template <typename T>
    concept trait = std::derived_from<T, Token>;

    CREEPER_DEFINE_CHECKER(trait);

}
using Grid = Declarative<grid::internal::Grid, grid::pro::checker>;
}
