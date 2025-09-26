#include "animatable.hh"
using namespace creeper;

#include <qtimer.h>
using qwidget = QWidget;
using qtimer  = QTimer;

struct Animatable::Impl {

    qwidget& component;
    qtimer scheduler;

    std::vector<std::unique_ptr<ITransitionTask>> transition_tasks;

    explicit Impl(auto& component, int hz = 90) noexcept
        : component { component } {
        scheduler.connect(&scheduler, &qtimer::timeout, [this] { update(); });
        scheduler.setInterval(1'000 / hz);
    }

    auto set_frame_rate(int hz) noexcept -> void { scheduler.setInterval(1'000 / hz); }

    auto append_transition_task(std::unique_ptr<ITransitionTask> task) noexcept -> void {
        transition_tasks.push_back(std::move(task));
        if (!scheduler.isActive()) scheduler.start();
    }

    auto update() noexcept -> void {

        const auto [first, last] = std::ranges::remove_if(transition_tasks,
            [](const std::unique_ptr<ITransitionTask>& task) { return task->update(); });

        component.update();

        transition_tasks.erase(first, last);

        if (transition_tasks.empty()) scheduler.stop();
    }
};

Animatable::Animatable(QWidget& component) noexcept
    : pimpl { std::make_unique<Impl>(component) } { }

Animatable::~Animatable() = default;

auto Animatable::set_frame_rate(int hz) noexcept -> void {
    //
    pimpl->set_frame_rate(hz);
}
auto Animatable::push_transition_task(std::unique_ptr<ITransitionTask> task) noexcept -> void {
    pimpl->append_transition_task(std::move(task));
}
