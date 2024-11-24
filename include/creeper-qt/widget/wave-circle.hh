#pragma once

#include "creeper-qt/utility/math.hh"
#include "creeper-qt/widget/widget.hh"

#include <qpainter.h>
#include <qpainterpath.h>
#include <qpicture.h>
#include <qwidget.h>

#include <ranges>

namespace creeper {

class WaveCircle : public Extension<QWidget> {
    Q_OBJECT
public:
    explicit WaveCircle(QWidget* parent = nullptr)
        : Extension(parent) {
        reloadTheme();
    }
    WaveCircle& setFlange(uint flange) {
        assert(flange > 0);
        flange_ = flange;
        renderRequest_ = true;
        return *this;
    }
    WaveCircle& setRadius(double radius) {
        radius_ = radius;
        renderRequest_ = true;
        setFixedSize(2 * radius, 2 * radius);
        return *this;
    }
    WaveCircle& setFlangeRatio(double ratio) {
        assert(radius > 0 && radius < 1);
        ratio_ = ratio;
        renderRequest_ = true;
        return *this;
    }
    WaveCircle& setFlangeRadius(double radius) {
        flangeRadius_ = radius;
        renderRequest_ = true;
        return *this;
    }
    WaveCircle& setLineColor(uint32_t color) {
        lineColor_ = color;
        renderRequest_ = true;
        return *this;
    }
    WaveCircle& setBackground(uint32_t color) {
        background_ = color;
        renderRequest_ = true;
        return *this;
    }
    WaveCircle& setLineWidth(double width) {
        lineWidth_ = width;
        renderRequest_ = true;
        return *this;
    }
    void reloadTheme() override {
        setLineColor(Theme::color("primary400"));
        renderRequest_ = true;
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        auto painter = QPainter(this);
        auto circle = QPicture();
        makeCanvas(circle);
        painter.drawPicture(0, 0, circle);
    }

    /// @note: 先用QPicture实现，Pixmap等之后在看看
    /// 帧缓存解决了之前重复计算的性能问题
    void makeCanvas(QPicture& picture) {
        static auto lastFrame = QPicture {};
        if (!renderRequest_) {
            picture = lastFrame;
            return;
        }

        auto painter = QPainter { &picture };
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(QPen { { lineColor_ }, lineWidth_, Qt::SolidLine, Qt::RoundCap });
        painter.setBrush({ background_ });
        painter.setOpacity(1);

        const auto center = QPointF(width() / 2., height() / 2.);
        const auto step = 2 * std::numbers::pi / flange_;
        const auto radius = 0.8 * radius_;

        std::vector<QPointF> outside(flange_ + 2), inside(flange_ + 2);
        for (auto&& [index, point] : std::views::enumerate(std::views::zip(outside, inside))) {
            auto& [outside, inside] = point;
            outside.setX(radius * std::cos(-index * step));
            outside.setY(radius * std::sin(-index * step));
            inside.setX(ratio_ * radius * std::cos(double(-index + 0.5) * step));
            inside.setY(ratio_ * radius * std::sin(double(-index + 0.5) * step));
        }

        auto begin = QPointF {};
        auto path = QPainterPath {};
        for (int index = 0; index < flange_; index++) {
            const auto convexAngle = RoundAngleSolution(center + outside[index],
                center + inside[index], center + inside[index + 1], flangeRadius_);
            const auto concaveAngle = RoundAngleSolution(center + inside[index + 1],
                center + outside[index + 1], center + outside[index], flangeRadius_);
            if (index == 0) begin = convexAngle.start, path.moveTo(begin);
            path.lineTo(convexAngle.start);
            path.arcTo(convexAngle.rect, convexAngle.angleStart, convexAngle.angleLength);
            path.lineTo(concaveAngle.end);
            path.arcTo(concaveAngle.rect, concaveAngle.angleStart + concaveAngle.angleLength,
                -concaveAngle.angleLength);
        }
        path.lineTo(begin);
        painter.drawPath(path);

        lastFrame = picture;
        renderRequest_ = false;
    }

private:
    uint flange_ = 12;
    double flangeRadius_ = 10;
    double radius_ = 100;
    double ratio_ = 0.8;

    double lineWidth_ = 3;
    uint32_t lineColor_;
    uint32_t background_;

    bool renderRequest_ = true;
};
}