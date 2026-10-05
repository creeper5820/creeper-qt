#pragma once

#include "creeper-qt/utility/api/scope/common.hh"
#include <creeper-qt/layout/group.hh>
#include <creeper-qt/utility/wrapper/layout.hh>
#include <ranges>

namespace creeper::mutual_exclusion_group {

template <auto f, typename W>
concept switch_function_trait = std::invocable<decltype(f), W&, bool>;

constexpr inline auto checked_switch_function  = [](auto& w, bool on) { w.setChecked(on); };
constexpr inline auto opened_switch_function   = [](auto& w, bool on) { w.setOpened(on); };
constexpr inline auto selected_switch_function = [](auto& w, bool on) { w.setSelected(on); };

}

namespace creeper {

template <layout_trait T, widget_trait W, auto switch_function>
    requires mutual_exclusion_group::switch_function_trait<switch_function, W>
class MutualExclusionGroup : public Group<T, W> {
    using Base = Group<T, W>;

public:
    using Base::Base;

    explicit MutualExclusionGroup(auto&&... props) {
        this->construct_with(std::forward<decltype(props)>(props)...);
    }

    auto switchWidgets(std::size_t index) const noexcept {
        for (auto [index_, w] : std::views::enumerate(this->widgets)) {
            switch_function(*w, index_ == index);
        }
    }
    auto switchWidgets(W* widget) const noexcept {
        for (auto w_ : this->widgets) {
            switch_function(*w_, w_ == widget);
        }
    }

    auto makeSignalInjection(auto signal) const noexcept -> void {
        for (auto widget : this->widgets) {
            QObject::connect(widget, signal, [this, widget] { switchWidgets(widget); });
        }
    }
};

}

namespace creeper::mutual_exclusion_group::pro {

using namespace common::pro;
using namespace api::scope::common;
using namespace group::pro;

template <typename Signal>
struct SignalInjection {
    Signal signal;

    explicit SignalInjection(Signal signal) noexcept
        : signal { signal } { }

    friend auto dsl_invoke(auto& self, const SignalInjection& prop) -> void {
        self.makeSignalInjection(prop.signal);
    }
};

}

namespace creeper {

template <layout_trait T, widget_trait W>
using CheckGroup      = MutualExclusionGroup<T, W, mutual_exclusion_group::checked_switch_function>;
namespace check_group = mutual_exclusion_group;

template <layout_trait T, widget_trait W>
using OpenGroup      = MutualExclusionGroup<T, W, mutual_exclusion_group::opened_switch_function>;
namespace open_group = mutual_exclusion_group;

template <layout_trait T, widget_trait W>
using SelectGroup = MutualExclusionGroup<T, W, mutual_exclusion_group::selected_switch_function>;
namespace select_group = mutual_exclusion_group;

}
