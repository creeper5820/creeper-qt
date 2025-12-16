// Generate by Gemini3
#pragma once

#include <qcoreapplication.h>
#include <qdebug.h>
#include <stdexec/execution.hpp>

// -----------------------------------------------------------------------------
// 1. Operation State
// 这是真正干活的地方。当 start() 被调用时，它触发 invokeMethod。
// -----------------------------------------------------------------------------
template <typename Receiver>
struct QtOperationState {
    QObject* context_;
    Receiver receiver_;

    // 必须处理接收器的移动，因为 invokeMethod 的 lambda 需要捕获它
    // 注意：标准的 Receiver 是 move-only 的，但 Qt 的一些旧版本 invokeMethod
    // 可能需要 copyable 的 functor。这里使用 shared_ptr<optional>
    // 是一种常见的绕过 functor 复制限制的技巧，或者使用 C++23 move-only-function。
    // 为了通用性，这里简化处理，假设环境支持 move lambda 或使用指针包装。

    friend void tag_invoke(stdexec::start_t, QtOperationState& self) noexcept {
        // 检查 context 是否还活着
        if (!self.context_) {
            stdexec::set_error(std::move(self.receiver_),
                std::make_exception_ptr(std::runtime_error("Context is null")));
            return;
        }

        // 使用 invokeMethod 将任务投递到 context 所在的线程
        // Qt::QueuedConnection 强制进入事件循环
        bool success = QMetaObject::invokeMethod(
            self.context_,
            [r = std::move(self.receiver_)]() mutable {
                // 在目标线程执行
                if (stdexec::get_stop_token(stdexec::get_env(r)).stop_requested()) {
                    stdexec::set_stopped(std::move(r));
                } else {
                    stdexec::set_value(std::move(r));
                }
            },
            Qt::QueuedConnection);

        if (!success) {
            // 如果投递失败（例如线程正在析构），抛出错误
            // 注意：此时 receiver 已经被 move 进 lambda，处理这种情况比较棘手。
            // 生产环境通常需要更复杂的生命周期管理 (如使用 shared_state)。
            qDebug() << "Failed to invoke method on Qt Object!";
        }
    }
};

// -----------------------------------------------------------------------------
// 2. Sender
// 描述“调度”这一动作。
// -----------------------------------------------------------------------------
struct QtScheduleSender {
    // 定义 Sender 的属性
    using sender_concept        = stdexec::sender_t;
    using completion_signatures = stdexec::completion_signatures<stdexec::set_value_t(),
        stdexec::set_error_t(std::exception_ptr), stdexec::set_stopped_t()>;

    QObject* context_;

    // 让 Receiver 能获取当前环境（例如查询这个 sender 属于哪个 scheduler）
    // 这里为了简化略去了 get_env 的实现，但在生产级代码中需要加上 query(get_completion_scheduler)。

    template <typename Receiver>
    friend auto tag_invoke(stdexec::connect_t, QtScheduleSender self, Receiver r) {
        return QtOperationState<Receiver> { self.context_, std::move(r) };
    }
};

// -----------------------------------------------------------------------------
// 3. Scheduler
// 用户持有的句柄。
// -----------------------------------------------------------------------------
struct QtScheduler {
    QObject* context_;

    // 必须实现比较运算符
    friend bool operator==(const QtScheduler& a, const QtScheduler& b) noexcept {
        return a.context_ == b.context_;
    }
    friend bool operator!=(const QtScheduler& a, const QtScheduler& b) noexcept {
        return !(a == b);
    }

    // Schedule CPO 实现
    friend QtScheduleSender tag_invoke(stdexec::schedule_t, const QtScheduler& self) noexcept {
        return QtScheduleSender { self.context_ };
    }

    QtScheduleSender schedule() const noexcept {
        // 内部转发给 tag_invoke 实现，确保逻辑一致
        return QtScheduleSender { this->context_ };
    }
};
