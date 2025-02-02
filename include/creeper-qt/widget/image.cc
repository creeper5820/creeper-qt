#include "image.hh"
#include <qpainter.h>
#include <qpainterpath.h>

using namespace creeper;

struct Image::Impl {
    double radius = 0;
    double borderWidth = 0;
    QPixmap pixmap;
    QColor borderColor = Qt::transparent;
    QColor background = Qt::transparent;
    Fitness fitness = Fitness::None;

    double last_width = 0;
    double last_height = 0;
    QPixmap last_pixmap;
    Fitness last_fitness = Fitness::None;

    QPixmap makeSuitablePixmap(double width, double height) {
        if (last_height == height && last_width == width && last_fitness == fitness)
            return last_pixmap;
        last_fitness = fitness;
        last_height = height;
        last_width = width;

        auto imageRectEnd = QPointF(pixmap.width(), pixmap.height());

        switch (fitness) {
        case Fitness::Contain:
            imageRectEnd *= width / imageRectEnd.x();
            if (imageRectEnd.y() > height) //
                imageRectEnd *= height / imageRectEnd.y();
            last_pixmap = pixmap.scaled(imageRectEnd.x(), imageRectEnd.y(),
                Qt::AspectRatioMode::IgnoreAspectRatio,
                Qt::TransformationMode::SmoothTransformation);
            break;

        case Fitness::Cover:
            imageRectEnd *= width / imageRectEnd.x();
            if (imageRectEnd.y() < height) //
                imageRectEnd *= height / imageRectEnd.y();
            last_pixmap = pixmap.scaled(imageRectEnd.x(), imageRectEnd.y(),
                Qt::AspectRatioMode::IgnoreAspectRatio,
                Qt::TransformationMode::SmoothTransformation);
            break;

        case Fitness::ScaleDown:
            if (imageRectEnd.x() > width) //
                imageRectEnd *= width / imageRectEnd.x();
            if (imageRectEnd.y() > height) //
                imageRectEnd *= height / imageRectEnd.y();
            last_pixmap = pixmap.scaled(imageRectEnd.x(), imageRectEnd.y(),
                Qt::AspectRatioMode::IgnoreAspectRatio,
                Qt::TransformationMode::SmoothTransformation);
            break;

        case Fitness::Fill:
            last_pixmap = pixmap.scaled(width, height, Qt::AspectRatioMode::IgnoreAspectRatio,
                Qt::TransformationMode::SmoothTransformation);
            break;

        case Fitness::None:
            last_pixmap = pixmap;
            break;
        }
        return last_pixmap;
    }
};

Image::Image(QWidget* parent)
    : pimpl_(new Impl)
    , QAbstractButton(parent) { }

Image::Image(const QString& path, QWidget* parent)
    : Image(parent) {
    setPixmap(path);
}

Image::Image(const QPixmap& pixmap, QWidget* parent)
    : Image(parent) {
    setPixmap(pixmap);
}

Image::~Image() { delete pimpl_; }

void Image::paintEvent(QPaintEvent* event) {
    const double radius = pimpl_->radius;
    const double width = this->width();
    const double height = this->height();

    auto painter = QPainter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // draw background
    painter.setPen(Qt::NoPen);
    painter.setBrush({ pimpl_->background });
    painter.drawRoundedRect(0, 0, width, height, radius, radius);

    // draw border
    if (pimpl_->borderWidth != 0) {
        painter.setBrush({ pimpl_->borderColor });
        painter.drawRoundedRect(0, 0, width, height, radius, radius);
    }

    // draw image
    const auto bw = pimpl_->borderWidth;
    const auto pixmap = pimpl_->makeSuitablePixmap(width - bw * 2, height - bw * 2);
    if (!pixmap.isNull()) {
        const auto pixmapPaintPoint
            = QPoint((width - pixmap.width()) / 2., (height - pixmap.height()) / 2.);
        auto pixmapPath = QPainterPath {};
        pixmapPath.addRoundedRect(
            bw, bw, width - 2 * bw, height - 2 * bw, radius - bw / 2, radius - bw / 2);

        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::NoBrush);
        painter.setClipPath(pixmapPath);
        painter.drawPixmap(pixmapPaintPoint, pixmap);
    }
}

Image& Image::setPixmap(const QPixmap& pixmap) {
    pimpl_->pixmap = pixmap;
    return *this;
}
Image& Image::setPixmap(const QString& path) {
    setPixmap(QPixmap(path));
    return *this;
}
Image& Image::setRadius(double radius) {
    pimpl_->radius = radius;
    return *this;
}
Image& Image::setBorderWidth(double width) {
    pimpl_->borderWidth = width;
    return *this;
}
Image& Image::setBorderColor(QColor color) {
    pimpl_->borderColor = color;
    return *this;
}
Image& Image::setBackground(QColor color) {
    pimpl_->background = color;
    return *this;
}
Image& Image::setFitness(Fitness fitness) {
    pimpl_->fitness = fitness;
    return *this;
}
