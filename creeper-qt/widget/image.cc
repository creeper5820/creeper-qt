#include "image.impl.hh"

Image::Image()
    : pimpl { std::make_unique<Impl>(*this) } { }

Image::~Image() = default;

auto Image::set_content_scale(ContentScale scale) noexcept -> void {
    pimpl->content_scale = scale;
    pimpl->request_scale = true;
}

auto Image::content_scale() const noexcept -> ContentScale { return pimpl->content_scale; }
