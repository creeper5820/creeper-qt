#include "image.impl.hh"

Image::Image()
    : pimpl { std::make_unique<Impl>(*this) } { }

Image::~Image() = default;

auto Image::update_pixmap() noexcept -> void {
    pimpl->request_regenerate = true;
    this->update();
}

auto Image::set_content_scale(ContentScale scale) noexcept -> void {
    pimpl->content_scale      = scale;
    pimpl->request_regenerate = true;
}
auto Image::content_scale() const noexcept -> ContentScale { return pimpl->content_scale; }

auto Image::set_painter_resource(std::unique_ptr<PainterResource> resource) noexcept -> void {
    pimpl->resource_origin    = std::move(resource);
    pimpl->request_regenerate = true;
}
auto Image::painter_resource() const noexcept -> PainterResource { return *pimpl->resource_origin; }

auto Image::set_opacity(double opacity) noexcept -> void { pimpl->opacity = opacity; }
auto Image::set_radius(double radius) noexcept -> void { pimpl->radius = radius; }
auto Image::set_border_width(double width) noexcept -> void { pimpl->border_width = width; }
auto Image::set_border_color(QColor color) noexcept -> void { pimpl->border_color = color; }

auto Image::paintEvent(QPaintEvent* event) -> void { pimpl->paint_event(*event); }
auto Image::resizeEvent(QResizeEvent* event) -> void { pimpl->resize_event(*event); }
