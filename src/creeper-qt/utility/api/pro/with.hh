#pragma once

#include <concepts>
#include <type_traits>

namespace creeper::api::pro {

template <typename Lambda>
struct With {
    Lambda&& lambda;

    explicit With(Lambda&& lambda) noexcept
        requires(!std::is_lvalue_reference<Lambda>::value)
        : lambda { static_cast<Lambda&&>(lambda) } { }

    template <typename T>
    friend auto dsl_invoke(T& self, With&& with)
        requires std::invocable<Lambda&, T&>
    {
        with.lambda(self);
    }
};

}
