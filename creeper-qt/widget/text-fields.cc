#include "text-fields.impl.hh"

BasicTextField::BasicTextField()
    : pimpl { std::make_unique<Impl>(*this) } { }

BasicTextField::~BasicTextField() = default;

auto BasicTextField::set_color_scheme(const ColorScheme& scheme) -> void { }

auto BasicTextField::load_theme_manager(ThemeManager& manager) -> void { }

auto BasicTextField::enterEvent(QEvent* event) -> void {
    QLineEdit::enterEvent(event); //
}

auto BasicTextField::leaveEvent(QEvent* event) -> void {
    QLineEdit::leaveEvent(event); //
}

auto BasicTextField::paintEvent(QPaintEvent* event) -> void {
    QLineEdit::paintEvent(event);
    pimpl->paint_event(*event);
}
