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

auto BasicTextField::set_label_text(const QString& text) -> void { pimpl->set_label_text(text); }

auto BasicTextField::set_leading_icon(const QString& code, const QString& font) -> void {
    pimpl->set_leading_icon(code, font);
}

auto BasicTextField::enterEvent(QEvent* event) -> void {
    pimpl->enter_event(event);
    QLineEdit::enterEvent(event);
}

auto BasicTextField::leaveEvent(QEvent* event) -> void {
    pimpl->leave_event(event);
    QLineEdit::leaveEvent(event);
}

auto BasicTextField::focusInEvent(QFocusEvent* event) -> void {
    pimpl->focus_in(event);
    QLineEdit::focusInEvent(event);
}

auto BasicTextField::focusOutEvent(QFocusEvent* event) -> void {
    QLineEdit::focusOutEvent(event);
    pimpl->focus_out(event);
}

auto FilledTextField::paintEvent(QPaintEvent* event) -> void {
    pimpl->paint_filled(event);
    QLineEdit::paintEvent(event);
}

auto OutlinedTextField::paintEvent(QPaintEvent* event) -> void {
    pimpl->paint_outlined(event);
    QLineEdit::paintEvent(event);
}
