#include "text-fields.impl.hh"

BasicTextField::BasicTextField()
    : pimpl { std::make_unique<Impl>(*this) } { }

BasicTextField::~BasicTextField() = default;

auto BasicTextField::set_color_scheme(const ColorScheme& scheme) -> void {
    pimpl->set_color_scheme(scheme);
}

auto BasicTextField::load_theme_manager(ThemeManager& manager) -> void {
    pimpl->load_theme_manager(manager);
}

auto BasicTextField::enterEvent(QEvent* event) -> void {
    QLineEdit::enterEvent(event); //
}

auto BasicTextField::leaveEvent(QEvent* event) -> void {
    QLineEdit::leaveEvent(event); //
}

auto FilledTextField::paintEvent(QPaintEvent* event) -> void {
    QLineEdit::paintEvent(event);
    //
}

auto OutlinedTextField::paintEvent(QPaintEvent* event) -> void {
    QLineEdit::paintEvent(event);
    //
}
