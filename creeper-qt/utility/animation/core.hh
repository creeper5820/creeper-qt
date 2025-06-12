#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include <qevent.h>
#include <qtimer.h>
#include <qwidget.h>

namespace creeper::util::animation {

struct IAnimation {
    virtual ~IAnimation() = default;
    virtual bool update(const QPaintEvent&) = 0;
};

class AnimationCore {
public:
    // 构造完毕后默认不开启，需要添加动画或者手动开启
    explicit AnimationCore(QWidget& widget, int hz = 60) {
        QObject::connect(&scheduler, &QTimer::timeout, [&widget] { widget.update(); });
        scheduler.setInterval(1'000 / hz);
    }

    // 添加动画，动画执行完毕后会自动销毁
    void append_animation(std::unique_ptr<IAnimation> animation) {
        animations.push_back(std::move(animation));
        if (!scheduler.isActive()) scheduler.start();
    }

    void paint_event(const QPaintEvent& event) {
        const auto [erase_first, erase_last] = std::ranges::remove_if(
            animations, [event](const std::unique_ptr<IAnimation>& animation) {
                return animation->update(event);
            });
        animations.erase(erase_first, erase_last);
        if (animations.empty()) scheduler.stop();
    }

    void set_interval(int hz) { scheduler.setInterval(hz); }

    // 不建议手动操作开关，程序会根据动画存量来处理
    void set_status(bool running) { running ? scheduler.start() : scheduler.stop(); }

private:
    QTimer scheduler;
    std::vector<std::unique_ptr<IAnimation>> animations;
};

}
