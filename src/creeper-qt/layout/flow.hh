#pragma once

#include "creeper-qt/utility/api/scope/common.hh" // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/layout.hh" // IWYU pragma: keep
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"

#include <qlayout.h>

namespace creeper {

class Flow : public QLayout, public DSL {
    CREEPER_PIMPL_DEFINITION(Flow)

public:
    using Item = QLayoutItem;

    explicit Flow(auto&&... props)
        : Flow { } {
        construct_with(std::forward<decltype(props)>(props)...);
    }

    auto addItem(Item*) -> void override;
    auto takeAt(int) -> Item* override;
    auto setGeometry(const QRect&) -> void override;

    auto expandingDirections() const -> Qt::Orientations override;
    auto hasHeightForWidth() const -> bool override;
    auto heightForWidth(int) const -> int override;

    auto itemAt(int) const -> Item* override;
    auto count() const -> int override;
    auto minimumSize() const -> QSize override;
    auto sizeHint() const -> QSize override;

public:
    auto setRowSpacing(int) noexcept -> void;
    auto rowSpacing() const noexcept -> int;

    auto setColSpacing(int) noexcept -> void;
    auto colSpacing() const noexcept -> int;

    auto setRowLimit(int) noexcept -> void;
    auto rowLimit() const noexcept -> int;
};

}
namespace creeper::flow::pro {
struct RowSpacing {
    std::int32_t value;

    explicit RowSpacing(std::int32_t v) noexcept
        : value { v } { }

    friend auto dsl_invoke(Flow& self, const RowSpacing& prop) -> void {
        self.setRowSpacing(prop.value);
    }
};
struct ColSpacing {
    std::int32_t value;

    explicit ColSpacing(std::int32_t v) noexcept
        : value { v } { }

    friend auto dsl_invoke(Flow& self, const ColSpacing& prop) -> void {
        self.setColSpacing(prop.value);
    }
};
struct RowLimit {
    std::int32_t value;

    explicit RowLimit(std::int32_t v) noexcept
        : value { v } { }

    friend auto dsl_invoke(Flow& self, const RowLimit& prop) -> void {
        self.setRowLimit(prop.value);
    }
};
using MainAxisSpacing   = RowSpacing;
using CrossAxisSpacing  = ColSpacing;
using MaxItemsInEachRow = RowLimit;

using namespace api::scope::common;
using namespace api::scope::layout;
}
