#pragma once

#include "creeper-qt/utility/trait/widget.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/forward_prop.hh"
#include "creeper-qt/utility/wrapper/layout.hh"

#include <qgridlayout.h>

namespace creeper {

class Grid : public QGridLayout, public DSL {
public:
    using QGridLayout::QGridLayout;

    explicit Grid(auto&&... args) { construct_with(std::forward<decltype(args)>(args)...); }
};

namespace grid::pro {

    /// 行间距：行沿垂直方向堆叠，对应 setVerticalSpacing
    using RowSpacing = ForwardProp<&QGridLayout::setVerticalSpacing>;

    /// 列间距：列沿水平方向排列，对应 setHorizontalSpacing
    using ColSpacing = ForwardProp<&QGridLayout::setHorizontalSpacing>;

    template <item_trait T>
    struct Item {
        using Align = Qt::Alignment;

        struct LayoutMethod {
            int row = 0, row_span = 0;
            int col = 0, col_span = 0;
            Align align;

            explicit LayoutMethod(int row, int col, Align align = { })
                : row { row }
                , col { col }
                , align { align } { }

            explicit LayoutMethod(int row, int row_span, int col, int col_span, Align align = { })
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

        friend auto dsl_invoke(QGridLayout& layout, const Item& prop) -> void {
            if (prop.method.col_span == 0) {
                if constexpr (std::is_convertible_v<T*, QWidget*>)
                    layout.addWidget(
                        prop.item_pointer, prop.method.row, prop.method.col, prop.method.align);
                if constexpr (std::is_convertible_v<T*, QLayout*>)
                    layout.addLayout(
                        prop.item_pointer, prop.method.row, prop.method.col, prop.method.align);
            } else {
                if constexpr (std::is_convertible_v<T*, QWidget*>)
                    layout.addWidget(prop.item_pointer, prop.method.row, prop.method.row_span,
                        prop.method.col, prop.method.col_span, prop.method.align);
                if constexpr (std::is_convertible_v<T*, QLayout*>)
                    layout.addLayout(prop.item_pointer, prop.method.row, prop.method.row_span,
                        prop.method.col, prop.method.col_span, prop.method.align);
            }
        }
    };

    struct Items {
        explicit Items() { }

        friend auto dsl_invoke(QGridLayout&, const Items&) -> void { }
    };

    using namespace layout::pro;
}

}
