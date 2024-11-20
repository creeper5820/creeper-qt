#pragma once

#include "../utility/math.hh"
#include "../widget/widget.hh"

#include <qpainter.h>
#include <qpainterpath.h>
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
    void setFlange(uint flange) {
        assert(flange > 0);
        flange_ = flange;
    }
    void setRadius(double radius) {
        radius_ = radius;
        setFixedSize(2 * radius, 2 * radius);
    }
    void setFlangeRatio(double ratio) {
        assert(radius > 0 && radius < 1);
        ratio_ = ratio;
    }
    void setFlangeRadius(double radius) { flangeRadius_ = radius; }

    void setLineColor(uint32_t color) { lineColor_ = color; }

    void setLineWidth(double width) { lineWidth_ = width; }

    void reloadTheme() override { setLineColor(Theme::color("primary400")); }

protected:
    void paintEvent(QPaintEvent* event) override {
        static int count;
        qDebug() << "wave circle render: " << count++;
        const auto center = QPoint(width() / 2, height() / 2);
        const auto step = 2 * std::numbers::pi / flange_;
        const auto radius = 0.8 * radius_;

        std::vector<QPoint> outside(flange_ + 2), inside(flange_ + 2);
        for (auto&& [index, point] : std::views::enumerate(std::views::zip(outside, inside))) {
            auto& [outside, inside] = point;
            outside.setX(radius * std::cos(index * step));
            outside.setY(radius * std::sin(index * step));
            inside.setX(ratio_ * radius * std::cos(double(index - 0.5) * step));
            inside.setY(ratio_ * radius * std::sin(double(index - 0.5) * step));
        }

        auto path = QPainterPath();
        for (int index = 0; index < flange_ + 1; index++) {
            const auto e0 = center + outside[index];
            const auto e1 = center + inside[index + 1];
            const auto e2 = center + inside[index];
            const auto arc0 = RoundAngleSolution(e0, e1, e2, flangeRadius_);
            if (index != 0) path.lineTo(arc0.end);
            path.moveTo(arc0.start);
            path.arcTo(arc0.rect, arc0.angleStart, arc0.angleLength);
            path.moveTo(arc0.start);
            const auto a0 = center + inside[index + 1];
            const auto a1 = center + outside[index];
            const auto a2 = center + outside[index + 1];
            const auto arc1 = RoundAngleSolution(a0, a1, a2, flangeRadius_);
            path.lineTo(arc1.start);
            path.arcTo(arc1.rect, arc1.angleStart, arc1.angleLength);
            path.moveTo(arc1.end);
        }

        auto painter = QPainter { this };
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(QPen { { lineColor_ }, lineWidth_, Qt::SolidLine, Qt::RoundCap });
        painter.setBrush(Qt::NoBrush);
        painter.setOpacity(1);
        painter.drawPath(path);
    }

private:
    uint flange_ = 12;
    double flangeRadius_ = 10;
    double radius_ = 100;
    double ratio_ = 0.8;

    double lineWidth_ = 3;
    uint32_t lineColor_;
};
}