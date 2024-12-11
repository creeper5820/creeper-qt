#pragma once

namespace creeper {
class PidModule {
public:
    PidModule() { }

    PidModule(float kp, float ki, float kd)
        : kp_(kp)
        , ki_(ki)
        , kd_(kd) { }

    float update(float target, float current) {
        float error = target - current;
        float output = kp_ * error + ki_ * last_error_ + kd_ * (error - last_error_);
        last_error_ = error;
        return output;
    }

private:
    float kp_ = 0;
    float ki_ = 0;
    float kd_ = 0;
    float last_error_ = 0;
};

template <typename T> inline T updateWithPid(T value, T target, double kp = 1) {
    return static_cast<T>(value) + kp * (static_cast<T>(target) - static_cast<T>(value));
}

template <typename T> inline T updateWithPidError(T value, T error, double kp = 1) {
    return static_cast<T>(value) + kp * static_cast<T>(error);
}

}