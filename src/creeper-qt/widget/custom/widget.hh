#pragma once

#include "creeper-qt/utility/api/scope/common.hh"

#include "creeper-qt/utility/api/scope/widget.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"

#include <concepts>
#include <memory>
#include <type_traits>
#include <utility>
#include <variant>

namespace creeper {

class CustomWidget : public QWidget, public DSL {
public:
    struct OnPaint {
        virtual auto paint(CustomWidget&) -> void = 0;
        virtual ~OnPaint() { }
    };

    using QWidget::QWidget;

    explicit CustomWidget(auto&&... args) { construct_with(std::forward<decltype(args)>(args)...); }

    std::unique_ptr<OnPaint> on_paint = nullptr;

    auto setOnPaint(std::unique_ptr<OnPaint> _on_paint) noexcept {
        //
        on_paint = std::move(_on_paint);
    }

    auto paintEvent(QPaintEvent* e) -> void override {
        if (on_paint != nullptr) {
            on_paint->paint(*this);
        }
    }
};

namespace custom::pro {
    /// @note:
    /// - std::invocable<F, CustomWidget&>
    /// - std::invocable<F, CustomWidget&, State&>
    template <typename State = std::monostate>
    struct OnPaint {

        template <typename F>
        struct Instantiated : public CustomWidget::OnPaint {
            std::decay_t<State> state;
            std::decay_t<F> on_paint;

            explicit Instantiated(F&& on_paint) noexcept
                : state { }
                , on_paint { std::forward<F>(on_paint) } { }

            explicit Instantiated(State&& state, F&& on_paint) noexcept
                : state { std::forward<State>(state) }
                , on_paint { std::forward<F>(on_paint) } { }

            ~Instantiated() override = default;

            auto paint(CustomWidget& widget) -> void override {
                if constexpr (std::same_as<State, std::monostate>) {
                    on_paint(widget);
                } else {
                    on_paint(widget, state);
                }
            }
        };
        std::unique_ptr<CustomWidget::OnPaint> on_paint;

        template <typename F>
            requires std::invocable<F, CustomWidget&>
        explicit OnPaint(F&& f) noexcept
            requires std::same_as<State, std::monostate>
            : on_paint { std::make_unique<Instantiated<F>>(std::forward<F>(f)) } { }

        template <typename F>
            requires std::invocable<F, CustomWidget&, State&>
        explicit OnPaint(F&& f) noexcept
            requires std::default_initializable<State>
            : on_paint { std::make_unique<Instantiated<F>>(std::forward<F>(f)) } { }

        template <typename F>
            requires std::invocable<F, CustomWidget&, State&>
        explicit OnPaint(State&& state, F&& f) noexcept
            requires std::movable<State>
            : on_paint { std::make_unique<Instantiated<F>>(
                  std::forward<State>(state), std::forward<F>(f)) } { }

        friend auto dsl_invoke(CustomWidget& self, auto&& prop) -> void
            requires std::same_as<std::remove_cvref_t<decltype(prop)>, OnPaint>
        {
            self.setOnPaint(std::move(prop.on_paint));
        }
    };

    using namespace api::scope::common;
    using namespace api::scope::widget;
}

}
