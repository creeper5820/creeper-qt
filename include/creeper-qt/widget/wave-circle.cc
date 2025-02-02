#include "wave-circle.hh"

#include "creeper-qt/utility/math.hh"

#include <qpainter.h>
#include <qpainterpath.h>
#include <qpicture.h>
#include <qwidget.h>

#include <ranges>

using namespace creeper;
struct WaveCircle::Impl {
public:
    /// @note: 先用QPicture实现，Pixmap等之后在看看
    /// 帧缓存解决了之前重复计算的性能问题
    void makeCanvas(QPicture& picture, int width, int height) {
        if (!renderRequest) {
            picture = lastFrame;
            return;
        }

        auto painter = QPainter { &picture };
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(QPen { { lineColor }, lineWidth, Qt::SolidLine, Qt::RoundCap });
        painter.setBrush({ background });
        painter.setOpacity(1);

        const auto center = QPointF(width / 2., height / 2.);
        const auto step = 2 * std::numbers::pi / flange;
        const auto radius = 0.8 * this->radius;

        std::vector<QPointF> outside(flange + 2), inside(flange + 2);
        for (auto&& [index, point] : std::views::enumerate(std::views::zip(outside, inside))) {
            auto& [outside, inside] = point;
            outside.setX(radius * std::cos(-index * step));
            outside.setY(radius * std::sin(-index * step));
            inside.setX(ratio * radius * std::cos(double(-index + 0.5) * step));
            inside.setY(ratio * radius * std::sin(double(-index + 0.5) * step));
        }

        auto begin = QPointF {};
        auto path = QPainterPath {};
        for (int index = 0; index < flange; index++) {
            const auto convexAngle = RoundAngleSolution(center + outside[index],
                center + inside[index], center + inside[index + 1], flangeRadius);
            const auto concaveAngle = RoundAngleSolution(center + inside[index + 1],
                center + outside[index + 1], center + outside[index], flangeRadius);
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
        renderRequest = false;
    }

public:
    uint flange = 12;
    double flangeRadius = 10;
    double radius = 100;
    double ratio = 0.8;

    double lineWidth = 3;
    uint32_t lineColor;
    uint32_t background;

    bool renderRequest = true;

    QPicture lastFrame;
};

WaveCircle::WaveCircle(QWidget* parent)
    : Extension(parent)
    , pimpl_(new Impl) {
    reloadTheme();
}

WaveCircle::~WaveCircle() { delete pimpl_; }

WaveCircle& WaveCircle::setFlange(uint flange) {
    assert(flange > 0);
    pimpl_->flange = flange;
    pimpl_->renderRequest = true;
    return *this;
}
WaveCircle& WaveCircle::setRadius(double radius) {
    pimpl_->radius = radius;
    pimpl_->renderRequest = true;
    setFixedSize(2 * radius, 2 * radius);
    return *this;
}
WaveCircle& WaveCircle::setFlangeRatio(double ratio) {
    assert(ratio > 0 && ratio < 1);
    pimpl_->ratio = ratio;
    pimpl_->renderRequest = true;
    return *this;
}
WaveCircle& WaveCircle::setFlangeRadius(double radius) {
    pimpl_->flangeRadius = radius;
    pimpl_->renderRequest = true;
    return *this;
}
WaveCircle& WaveCircle::setLineColor(uint32_t color) {
    pimpl_->lineColor = color;
    pimpl_->renderRequest = true;
    return *this;
}
WaveCircle& WaveCircle::setBackground(uint32_t color) {
    pimpl_->background = color;
    pimpl_->renderRequest = true;
    return *this;
}
WaveCircle& WaveCircle::setLineWidth(double width) {
    pimpl_->lineWidth = width;
    pimpl_->renderRequest = true;
    return *this;
}

void WaveCircle::reloadTheme() { pimpl_->renderRequest = true; }

void WaveCircle::paintEvent(QPaintEvent* event) {
    auto painter = QPainter(this);
    auto circle = QPicture();
    pimpl_->makeCanvas(circle, width(), height());
    painter.drawPicture(0, 0, circle);
}
