#include <creeper-qt/utility/co/meta/coroutine.hh>

#include <cassert>
#include <print>
#include <stdexcept>

using namespace creeper::co;

// 统计帧内局部变量的存活数：帧被销毁时必须归零
struct Tracker {
    static inline int alive = 0;

    Tracker() { ++alive; }
    Tracker(const Tracker&) { ++alive; }
    Tracker(Tracker&&) noexcept { ++alive; }
    ~Tracker() { --alive; }
};

// 场景 1：挂起中析构 —— 帧内局部变量必须随帧销毁
auto suspend_forever() -> coroutine<> {
    auto tracker = Tracker {};
    co_await std::suspend_always {};
    co_return;
}

// 场景 2：正常跑完 —— final_suspend 挂起，帧由 coroutine 对象析构回收
auto complete_with_locals() -> coroutine<int> {
    auto tracker = Tracker {};
    co_return 1;
}

// 场景 3：co_await 组合 —— 子任务临时对象在语句结束时销毁子帧
auto child() -> coroutine<int> {
    auto tracker = Tracker {};
    co_return 7;
}
auto parent() -> coroutine<int> {
    auto tracker = Tracker {};
    auto r       = co_await child();
    assert(Tracker::alive == 1); // 只剩父帧
    co_return *r;
}

// 场景 4：异常路径 —— 帧同样由析构回收
auto throwing() -> coroutine<> {
    auto tracker = Tracker {};
    throw std::runtime_error { "boom" };
    co_return;
}

auto main() -> int {
    {
        auto task = suspend_forever();
        assert(Tracker::alive == 0); // 懒启动：体未执行
        task.resume();
        assert(Tracker::alive == 1); // 挂起中，局部变量活着
    }
    assert(Tracker::alive == 0); // 析构回收帧

    {
        auto task = complete_with_locals();
        task.resume();
        assert(task.done());
        assert(Tracker::alive == 0); // co_return 已销毁局部变量，帧仍停在 final_suspend
        assert(task.handle.promise().value == 1); // 帧仍存活：promise 值可读
    }
    assert(Tracker::alive == 0);

    {
        auto task = parent();
        task.resume();
        assert(task.done());
        assert(task.handle.promise().value == 7);
        assert(Tracker::alive == 0); // 父帧停在 final_suspend，但其局部变量已随 co_return 销毁
    }
    assert(Tracker::alive == 0);

    {
        auto task = throwing();
        task.resume();
        assert(task.done());
        assert(!task.handle.promise().value.has_value());
        assert(Tracker::alive == 0); // 异常传播时局部变量已被栈展开销毁
    }
    assert(Tracker::alive == 0);

    // 场景 5：move 构造转移所有权，源不再持有帧
    {
        auto a = suspend_forever();
        a.resume();
        auto b = std::move(a);
        assert(!a.handle);
        assert(Tracker::alive == 1);
    }
    assert(Tracker::alive == 0);

    // 场景 6：move 赋值先销毁自身旧帧，再接管新帧
    {
        auto a = suspend_forever();
        auto b = suspend_forever();
        a.resume();
        b.resume();
        assert(Tracker::alive == 2);
        b = std::move(a);
        assert(Tracker::alive == 1);
    }
    assert(Tracker::alive == 0);

    std::println("co-lifecycle: all ok");
    return 0;
}
