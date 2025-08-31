#pragma once
#include <creeper-qt/layout/group.hh>
#include <creeper-qt/utility/wrapper/layout.hh>
#include <ranges>

namespace creeper::mutual_exclusion_group::internal {

template <auto f, typename W>
concept switch_function_trait = std::invocable<decltype(f), W&, bool>;

template <layout_trait T, widget_trait W, auto switch_function>
    requires switch_function_trait<switch_function, W>
struct MutualExclusionGroup : public Group<T, W> {
    using Group<T, W>::Group;

public:
    auto switch_widgets(std::size_t index) const noexcept {
        for (auto [index_, w] : std::views::enumerate(this->widgets)) {
            switch_function(*w, index_ == index);
        }
    }
    auto switch_widgets(W* widget) const noexcept {
        for (auto w_ : this->widgets) {
            switch_function(*w_, w_ == widget);
        }
    }

    auto make_signal_injection(auto signal) const noexcept -> void {
        for (auto widget : this->widgets) {
            QObject::connect(widget, signal, [this, widget] { switch_widgets(widget); });
        }
    }
};

constexpr inline auto checked_switch_function  = [](auto& w, bool on) { w.set_checked(on); };
constexpr inline auto opened_switch_function   = [](auto& w, bool on) { w.set_opened(on); };
constexpr inline auto selected_switch_function = [](auto& w, bool on) { w.set_selected(on); };

constexpr inline auto no_action_as_token = [](auto& w, bool on) { };

}
namespace creeper::mutual_exclusion_group::pro {

using Token =
    common::Token<internal::MutualExclusionGroup<QLayout, QWidget, internal::no_action_as_token>>;

template <typename T>
concept trait = std::derived_from<T, Token> || group::pro::trait<T>;

template <typename Signal>
struct SignalInjection : Token {
    Signal signal;

    explicit SignalInjection(Signal signal) noexcept
        : signal { signal } { }

    auto apply(auto& self) const noexcept -> void {
        self.make_signal_injection(signal); //
    }
};

CREEPER_DEFINE_CHECKER(trait);
using namespace group::pro;
}
namespace creeper {

template <layout_trait T, widget_trait W, auto switch_function>
    requires mutual_exclusion_group::internal::switch_function_trait<switch_function, W>
using MutualEXclusionGroup =
    mutual_exclusion_group::internal::MutualExclusionGroup<T, W, switch_function>;

template <layout_trait T, widget_trait W>
using CheckGroup = Declarative<
    MutualEXclusionGroup<T, W, mutual_exclusion_group::internal::checked_switch_function>,
    CheckerOr<mutual_exclusion_group::pro::checker, typename T::Checker>>;
namespace check_group = mutual_exclusion_group;

template <layout_trait T, widget_trait W>
using OpenGroup = Declarative<
    MutualEXclusionGroup<T, W, mutual_exclusion_group::internal::opened_switch_function>,
    CheckerOr<mutual_exclusion_group::pro::checker, typename T::Checker>>;
namespace open_group = mutual_exclusion_group;

template <layout_trait T, widget_trait W>
using SelectGroup = Declarative<
    MutualEXclusionGroup<T, W, mutual_exclusion_group::internal::selected_switch_function>,
    CheckerOr<mutual_exclusion_group::pro::checker, typename T::Checker>>;
namespace select_group = mutual_exclusion_group;

namespace internal {
    inline auto use_mutual_exclusion_group_namespace() {
        std::ignore = check_group::pro::Token {};
        std::ignore = open_group::pro::Token {};
        std::ignore = select_group::pro::Token {};
    }
}

}
