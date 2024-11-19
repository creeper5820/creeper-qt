#pragma once

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

        std::vector<QPoint> outside(flange_), inside(flange_);
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
        painter.setPen(QPen { { Theme::color("primary600") }, 5, Qt::SolidLine, Qt::RoundCap });

        auto o0 = Eigen::Vector2d { 0, 30 };
        auto e00 = Eigen::Vector2d { 0, 60 };
        auto e01 = Eigen::Vector2d { 30, 30 };
        auto [rect0, start0, length0] = solveArcBetweenLines(30, o0, e00, e01);

        auto o1 = Eigen::Vector2d { 60, 30 };
        auto e10 = Eigen::Vector2d { 60, 0 };
        auto e11 = Eigen::Vector2d { 30, 30 };
        auto [rect1, start1, length1] = solveArcBetweenLines(30, o1, e10, e11);

        path.moveTo(0, 0);
        path.arcTo(rect0, start0, length0);
        path.arcTo(rect1, start1, length1);

        painter.drawPath(path);
    }

private:
    // 逆时针
    std::tuple<QRectF, double, double> solveArcBetweenLines(
        double radius, Eigen::Vector2d origin, Eigen::Vector2d end0, Eigen::Vector2d end1) {

        // solve the arc origin
        const auto v0 = Eigen::Vector2d { end0 - origin };
        const auto v1 = Eigen::Vector2d { end1 - origin };
        const auto dot = v0.x() * v1.x() + v0.y() * v1.y();
        const auto det = v0.x() * v1.y() - v0.y() * v1.x();
        const auto angle = std::abs(std::atan2(det, dot));
        qDebug() << "angle: " << angle;

        const auto width = radius / std::tan(angle / 2);
        qDebug() << "width: " << width;
        const auto verticalPoint = Eigen::Vector2d { origin + width * v0.normalized() };
        qDebug() << "vertical point: " << verticalPoint.x() << verticalPoint.y();

        const auto arcOrigin = Eigen::Vector2d { verticalPoint - radius * v0.unitOrthogonal() };
        qDebug() << "arc origin: " << arcOrigin.x() << arcOrigin.y();

        // solve the arc angle
        const auto angleStart = std::atan2(v0.unitOrthogonal().y(), v0.unitOrthogonal().x());
        const auto angleEnd = std::atan2(v1.unitOrthogonal().y(), v1.unitOrthogonal().x());
        const auto angleLength = angleEnd - angleStart;

        // solve the rect corners
        const auto v2 = Eigen::Vector2d { origin - arcOrigin }.normalized();
        const auto v3 = Eigen::Vector2d { v2.unitOrthogonal() };

        const auto corner0 = Eigen::Vector2d { arcOrigin + Eigen::Vector2d::UnitX() * radius
            + Eigen::Vector2d::UnitY() * radius };
        const auto corner1 = Eigen::Vector2d { arcOrigin - Eigen::Vector2d::UnitX() * radius
            - Eigen::Vector2d::UnitY() * radius };

        const auto arcRect
            = QRectF { QPointF(corner1.x(), corner1.y()), QPointF(corner0.x(), corner0.y()) };

        using std::numbers::pi;
        return { arcRect, angleStart * 180 / pi, angleLength * 180 / pi };
    }

private:
    int flange_ = 2;
    int radius_ = 100;
    double ratio_ = 0.8;
};
}