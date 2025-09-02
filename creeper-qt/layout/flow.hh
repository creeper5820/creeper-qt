#pragma once
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/layout.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include <qlayout.h>

namespace creeper::flow::internal {

class Flow : public QLayout {
    CREEPER_PIMPL_DEFINITION(Flow)

public:
    using Item = QLayoutItem;

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
    auto set_row_spacing(int) noexcept -> void;
    auto row_spacing() const noexcept -> int;

    auto set_col_spacing(int) noexcept -> void;
    auto col_spacing() const noexcept -> int;

    auto set_row_limit(int) noexcept -> void;
    auto row_limit() const noexcept -> int;
};

}
namespace creeper::flow::pro {

using Token = common::Token<internal::Flow>;

using RowSpacing = SetterProp<Token, int, [](auto& self, int v) { self.set_row_spacing(v); }>;

using ColSpacing = SetterProp<Token, int, [](auto& self, int v) { self.set_col_spacing(v); }>;

using RowLimit = SetterProp<Token, int, [](auto& self, int v) { self.set_row_limit(v); }>;

using MainAxisSpacing   = RowSpacing;
using CrossAxisSpacing  = ColSpacing;
using MaxItemsInEachRow = RowLimit;

template <class T>
concept trait = std::derived_from<T, Token>;

CREEPER_DEFINE_CHECKER(trait);
using namespace layout::pro;
}
namespace creeper {

using Flow = Declarative<flow::internal::Flow, CheckerOr<flow::pro::checker, layout::pro::checker>>;

}
