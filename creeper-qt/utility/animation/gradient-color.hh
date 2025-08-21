#pragma once

#include "creeper-qt/utility/animation/core.hh"

#include <eigen3/Eigen/Dense>
#include <qdebug.h>

namespace creeper::animate {

struct GradientColor : IAnimation {
private:
    using RGBA = Eigen::Vector4d;

    RGBA begin;
    RGBA final;

    double ratio;

    std::function<bool(const QColor&)> update_callback;

public:
    /// @param begin 初始颜色
    /// @param  目标颜色
    /// @param ratio 线性插值参数，值越小变化越慢
    /// @param update_callback 更新回调，返回 true 时立刻销毁该动画
    GradientColor(const QColor& begin, const QColor& final, double ratio,
        const std::function<bool(const QColor&)>& update_callback)
        : begin(from_color(begin))
        , final(from_color(final))
        , ratio(ratio)
        , update_callback(update_callback) { }

private:
    bool update() override {
        begin = lerp(begin, final, ratio);
        return (begin - final).cwiseAbs().maxCoeff() < 1e-1 || update_callback(from_vector4(begin));
    }

    static Eigen::Vector4d lerp(const Eigen::Vector4d& v1, const Eigen::Vector4d& v2, double t) {
        return (1.0 - t) * v1 + t * v2;
    }
    static Eigen::Vector4d from_color(const QColor& color) {
        return Eigen::Vector4d(color.red(), color.green(), color.blue(), color.alpha());
    }
    static QColor from_vector4(const Eigen::Vector4d& vector) {
        return QColor(vector[0], vector[1], vector[2], vector[3]);
    }
};

}
