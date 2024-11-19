#pragma once

#include "../utility/math.hh"
#include "../widget/widget.hh"

#include <eigen3/Eigen/Eigen>

#include <qpainter.h>
#include <qpainterpath.h>

#include <ranges>

namespace creeper {

class WaveCircle : public Extension<QWidget> {
    Q_OBJECT
public:
    explicit WaveCircle(QWidget* parent = nullptr)
        : Extension(parent) { }

    void setRadius(int radius) {
        radius_ = radius;
        setFixedSize(2 * radius, 2 * radius);
    }
    void setFlange(int flange) {
        assert(flange > 0);
        flange_ = flange;
    }
    void setFlangeRatio(double ratio) {
        assert(radius > 0 && radius < 1);
        ratio_ = ratio;
    }
    void reloadTheme() override { }

protected:
    void paintEvent(QPaintEvent* event) override {
        const auto bottomColor = Theme::color("primary100");
        const auto center = QPoint(width() / 2, height() / 2);
        const auto step = 2 * std::numbers::pi / flange_;
        const auto radius = 0.8 * radius_;

        std::vector<QPoint> outside(flange_ + 2), inside(flange_ + 2);
        for (auto&& [index, point] : std::views::enumerate(std::views::zip(outside, inside))) {
            auto& [outside, inside] = point;
            outside.setX(radius * std::cos(index * step));
            outside.setY(radius * std::sin(index * step));
            inside.setX(ratio_ * radius * std::cos((index + 0.5) * step));
            inside.setY(ratio_ * radius * std::sin((index + 0.5) * step));
        }

        auto painter = QPainter { this };
        painter.setPen(Qt::NoPen);
        painter.setBrush({ bottomColor });
        painter.setOpacity(1);
        painter.setRenderHint(QPainter::Antialiasing, true);

        auto path = QPainterPath();
        // path.moveTo(center + outside[0]);
        // for (auto&& [outside, inside] : std::views::zip(outside, inside)) {
        //     path.lineTo(center + outside);
        //     path.lineTo(center + inside);
        // }
        // path.lineTo(center + outside[0]);
        // path.closeSubpath();

        // painter.drawPath(path);

        painter.drawRect(0, 0, 60, 60);

        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen { { Theme::color("primary600") }, 2, Qt::SolidLine, Qt::RoundCap });

        auto arc0 = ArcBetweenLinesSolution(Eigen::Vector2d { 0, 30 }, { 0, 60 }, { 30, 30 }, 30);
        auto arc1 = ArcBetweenLinesSolution(Eigen::Vector2d { 60, 30 }, { 60, 0 }, { 30, 30 }, 30);

        path.moveTo(arc0.pointStart);
        path.arcTo(arc0.rect, arc0.angleStart, arc0.angleLength);
        path.moveTo(arc1.pointStart);
        path.arcTo(arc1.rect, arc1.angleStart, arc1.angleLength);

        painter.drawPath(path);
    }

private:
    int flange_ = 2;
    int radius_ = 100;
    double ratio_ = 0.8;
};
}