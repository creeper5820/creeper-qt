#pragma once

#include <functional>
#include <qwidget.h>

namespace creeper {

/// @brief 响应式属性包装器，允许属性变更后自动同步到所有已绑定组件。
///
/// 该模板用于包装具备 apply 方法的属性类型，使其具备响应式能力。
/// 通过 Mutable 包装后的属性可以被多个组件共享，
/// 当属性发生变更时，所有通过 Mutable 绑定的组件会自动更新自身属性，
/// 达到类似“全局属性联动”的效果。
///
/// Usage：
///   auto mutable_size = Mutable{widget::pro::FixedSize{100, 100}};
///   auto card_1 = OutlinedCard{mutable_size};
///   auto card_2 = OutlinedCard{mutable_size};
///   // 修改 mutable_size，card1/card2 会同步变化
///   mutable_size = {mutable_size.width() + 10, mutable_size.height() + 10};
///
/// @note
///   1. 不支持将 Mutable 实例放入 std::tuple 进行批量 apply，
///      必须显式调用 apply 绑定组件。
///      若允许混入 tuple，容易导致属性的响应式副作用被隐式传播，
///      使得属性绑定和变更难以追踪，带来较大维护隐患。
///      显式 apply 可以让响应式属性的边界清晰可控，
///      便于代码审查和后期维护。
///
///   2. Mutable 不支持拷贝和移动构造，需确保唯一性和生命周期安全。
///
///   3. 绑定对象被销毁时，内部回调会自动解绑，无内存泄漏风险。
///
/// @date 2025.7.28
template <class T> struct Mutable : public T {
public:
    explicit Mutable(const T& t) noexcept
        : T { t } { }

    Mutable(const Mutable&) = delete;
    Mutable(Mutable&&)      = delete;

    auto apply(auto& self) const noexcept -> void {
        T::apply(self);
        Mutable::bind(self);
    }
    auto bind(auto& self) const noexcept -> void {
        callbacks_.insert({
            &self,
            [&](const T& pro) { pro.apply(self); },
        });
        QObject::connect(&self, &QWidget::destroyed,
            [pointer = &self, this] //
            { callbacks_.erase(pointer); });
    }
    auto set(const T& t) noexcept -> void {
        static_cast<T&>(*this) = t;
        for (const auto& [_, f] : callbacks_)
            f(*this);
    }
    auto operator=(const T& t) noexcept -> Mutable<T>& {
        set(t);
        return *this;
    }

private:
    mutable std::unordered_map<QWidget*, std::function<void(const T&)>> callbacks_;
};

}
