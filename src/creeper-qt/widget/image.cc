#include "image.impl.hh"

using namespace creeper;

Image::Image()
    : pimpl { std::make_unique<Impl>(*this) } { }

Image::~Image() = default;

auto Image::updatePixmap() noexcept -> void {
    pimpl->request_regenerate = true;
    this->update();
}

auto Image::setContentScale(ContentScale scale) noexcept -> void {
    pimpl->content_scale      = scale;
    pimpl->request_regenerate = true;
}
auto Image::contentScale() const noexcept -> ContentScale { return pimpl->content_scale; }

auto Image::setPainterResource(std::shared_ptr<PainterResource> resource) noexcept -> void {
    pimpl->resource_origin = std::move(resource);
    pimpl->resource_origin->add_finished_callback([this](auto&) { update(); });
    pimpl->request_regenerate = true;
}
auto Image::painterResource() const noexcept -> PainterResource { return *pimpl->resource_origin; }

auto Image::setOpacity(double opacity) noexcept -> void {
    pimpl->opacity = opacity;
    update();
}
auto Image::setRadius(double radius) noexcept -> void {
    pimpl->radius = radius;
    update();
}
auto Image::setBorderWidth(double width) noexcept -> void {
    pimpl->border_width = width;
    update();
}
auto Image::setBorderColor(QColor color) noexcept -> void {
    pimpl->border_color = color;
    update();
}

auto Image::paintEvent(QPaintEvent* event) -> void { pimpl->paint_event(*event); }
auto Image::resizeEvent(QResizeEvent* event) -> void { pimpl->resize_event(*event); }
