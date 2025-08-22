#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include <qevent.h>
#include <qtimer.h>
#include <qwidget.h>

namespace creeper::animate {

class AnimationCore;

struct IAnimation {
    virtual ~IAnimation() = default;
    virtual bool update() = 0;
};

class AnimationCore {
public:
    // 推荐帧率
    static constexpr auto kHz = 90;

    // 构造完毕后默认不开启，需要添加动画或者手动开启
    explicit AnimationCore(std::function<void()> update_complete_callback, int hz = 90)
        : update_complete_callback_ { std::move(update_complete_callback) } {
        QObject::connect(&scheduler_, &QTimer::timeout, [this] {
            const auto [first, last] = std::ranges::remove_if(animations_,
                [](const std::unique_ptr<IAnimation>& animation) { return animation->update(); });
            animations_.erase(first, last);
            if (animations_.empty()) scheduler_.stop();
            update_complete_callback_();
        });
        scheduler_.setInterval(1'000 / hz);
    }

    // 添加动画，动画执行完毕后会自动销毁
    void append(std::unique_ptr<IAnimation> animation) {
        animations_.push_back(std::move(animation));
        if (!scheduler_.isActive()) scheduler_.start();
    }

    // 设置动画帧率
    void set_interval(int hz) { scheduler_.setInterval(hz); }

private:
    QTimer scheduler_;
    std::vector<std::unique_ptr<IAnimation>> animations_;
    std::function<void()> update_complete_callback_;

    void set_status(bool running) { running ? scheduler_.start() : scheduler_.stop(); }
};

}
