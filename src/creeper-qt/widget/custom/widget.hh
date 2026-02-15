#pragma once
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/widget.hh"

namespace creeper::custom::details {

struct CustomWidget : public QWidget {

    struct OnPaint {
        virtual auto paint(CustomWidget&) -> void = 0;
        virtual ~OnPaint() { }
    };

    std::unique_ptr<OnPaint> on_paint = nullptr;

    auto set_on_paint(std::unique_ptr<OnPaint> _on_paint) noexcept {
        //
        on_paint = std::move(_on_paint);
    }

    auto paintEvent(QPaintEvent* e) -> void override {
        if (on_paint != nullptr) {
            on_paint->paint(*this);
        }
    }
};

}
namespace creeper::custom::pro {

using Token = common::Token<details::CustomWidget>;

/// @note:
/// - std::invocable<F, QPaintEvent&, State&>
/// - std::invocable<F, QPaintEvent&>
template <typename State = std::monostate>
struct OnPaint : Token {

    template <typename F>
    struct Instantiated : public details::CustomWidget::OnPaint {
        std::decay_t<State> state;
        std::decay_t<F> on_paint;

        explicit Instantiated(F&& on_paint) noexcept
            : state {}
            , on_paint { std::forward<F>(on_paint) } { }

        explicit Instantiated(State&& state, F&& on_paint) noexcept
            : state { std::forward<State>(state) }
            , on_paint { std::forward<F>(on_paint) } { }

        ~Instantiated() override = default;

        auto paint(details::CustomWidget& widget) -> void override {
            if constexpr (std::same_as<State, std::monostate>) {
                on_paint(widget);
            } else {
                on_paint(widget, state);
            }
        }
    };
    std::unique_ptr<details::CustomWidget::OnPaint> on_paint;

    template <typename F>
        requires std::invocable<F, details::CustomWidget&>
    explicit OnPaint(F&& f) noexcept
        requires std::same_as<State, std::monostate>
        : on_paint { std::make_unique<Instantiated<F>>(std::forward<F>(f)) } { }

    template <typename F>
        requires std::invocable<F, details::CustomWidget&, State&>
    explicit OnPaint(F&& f) noexcept
        requires std::default_initializable<State>
        : on_paint { std::make_unique<Instantiated<F>>(std::forward<F>(f)) } { }

    template <typename F>
        requires std::invocable<F, details::CustomWidget&, State&>
    explicit OnPaint(State&& state, F&& f) noexcept
        requires std::movable<State>
        : on_paint { std::make_unique<Instantiated<F>>(
              std::forward<State>(state), std::forward<F>(f)) } { }

    auto apply(auto& widget) { widget.set_on_paint(std::move(on_paint)); }
};

template <class T>
concept trait = std::derived_from<T, Token>;
CREEPER_DEFINE_CHECKER(trait);

using namespace widget::pro;
}
namespace creeper {
using CustomWidget = Declarative<custom::details::CustomWidget,
    CheckerOr<custom::pro::checker, widget::pro::checker>>;
}
