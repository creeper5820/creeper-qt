#pragma once

#include "../widget/widget.hh"

#include <eigen3/Eigen/Eigen>
#include <qpainter.h>

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
        using std::numbers::pi;
        const auto center = Eigen::Vector2d(width() / 2, height() / 2);
        const auto angleStep = 2 * pi / flange_;
        const auto radius = 0.8 * radius_;

        auto outsideVertex = std::vector<Eigen::Vector2d>(flange_);
        for (int index = 0; auto& point : outsideVertex) {
            point.x() = radius * std::cos(index * angleStep);
            point.y() = radius * std::sin(index * angleStep);
            index++;
        }
        auto insideVertex = std::vector<Eigen::Vector2d>(flange_);
        for (int index = 0; auto& point : insideVertex) {
            point.x() = 0.8 * radius * std::cos((index + 0.5) * angleStep);
            point.y() = 0.8 * radius * std::sin((index + 0.5) * angleStep);
            index++;
        }

        auto painter = QPainter { this };
        painter.setPen(QPen(Qt::black, 5, Qt::SolidLine, Qt::RoundCap));
        painter.setBrush(Qt::black);
        painter.setOpacity(1);

        for (const auto outside : outsideVertex) {
            const auto drawPoint = center + outside;
            painter.drawPoint(drawPoint.x(), drawPoint.y());
        }
        for (const auto inside : insideVertex) {
            const auto drawPoint = center + inside;
            painter.drawPoint(drawPoint.x(), drawPoint.y());
        }
    }

private:
    int flange_ = 2;
    int radius_;
    double ratio_ = 0;
};

}