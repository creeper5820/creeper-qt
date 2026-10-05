#pragma once

#include "creeper-qt/utility/api/scope/common.hh" // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/layout.hh" // IWYU pragma: keep
#include "creeper-qt/utility/trait/widget.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/forward_prop.hh"

#include <qgridlayout.h>

#include <type_traits>

namespace creeper {

class Grid : public QGridLayout, public DSL {
public:
    using QGridLayout::QGridLayout;

    struct Placement {
        int row, row_span { };
        int col, col_span { };
        Qt::Alignment align;

        explicit Placement(int row, int col, Qt::Alignment align = { })
            : row { row }
            , col { col }
            , align { align } { }

        explicit Placement(int row, int row_span, int col, int col_span, Qt::Alignment align = { })
            : row { row }
            , col { col }
            , row_span { row_span }
            , col_span { col_span }
            , align { align } { }
    };

    explicit Grid(auto&&... args) { construct_with(std::forward<decltype(args)>(args)...); }
};

namespace grid::pro {
    /// 行间距：行沿垂直方向堆叠，对应 setVerticalSpacing
    using RowSpacing = ForwardProp<&QGridLayout::setVerticalSpacing>;
    /// 列间距：列沿水平方向排列，对应 setHorizontalSpacing
    using ColSpacing = ForwardProp<&QGridLayout::setHorizontalSpacing>;

    template <item_trait T>
    struct GridItem {
        Grid::Placement placement;
        T* item_pointer = nullptr;

        explicit GridItem(const Grid::Placement& placement, T* pointer) noexcept
            : placement { placement }
            , item_pointer { pointer } { }

        template <typename... Args>
            requires std::constructible_from<T, Args...>
        explicit GridItem(const Grid::Placement& placement, Args&&... args) noexcept
            : placement { placement }
            , item_pointer { new T { std::forward<Args>(args)... } } { }

        friend auto dsl_invoke(QGridLayout& layout, const GridItem& prop) -> void {
            const auto& placement = prop.placement;
            if (placement.col_span == 0) {
                if constexpr (std::is_convertible_v<T*, QWidget*>)
                    layout.addWidget(
                        prop.item_pointer, placement.row, placement.col, placement.align);
                if constexpr (std::is_convertible_v<T*, QLayout*>)
                    layout.addLayout(
                        prop.item_pointer, placement.row, placement.col, placement.align);
            } else {
                if constexpr (std::is_convertible_v<T*, QWidget*>)
                    layout.addWidget(prop.item_pointer, placement.row, placement.row_span,
                        placement.col, placement.col_span, placement.align);
                if constexpr (std::is_convertible_v<T*, QLayout*>)
                    layout.addLayout(prop.item_pointer, placement.row, placement.row_span,
                        placement.col, placement.col_span, placement.align);
            }
        }
    };

    using namespace api::scope::common;
    using namespace api::scope::layout;
}

template <item_trait W>
auto operator+(W* item, Grid::Placement placement) -> grid::pro::GridItem<W> {
    return grid::pro::GridItem<W> { placement, item };
}

}
