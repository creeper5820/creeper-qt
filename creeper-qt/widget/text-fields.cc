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

auto BasicTextField::set_label_text(const QString& text) -> void {
    pimpl->set_label_text(text); //
}

auto BasicTextField::set_hint_text(const QString& text) -> void { }

auto BasicTextField::set_supporting_text(const QString& text) -> void { }

auto BasicTextField::set_leading_icon(const QIcon& text) -> void { }

auto BasicTextField::set_leading_icon(const QString& code, const QString& font) -> void {
    pimpl->set_leading_icon(code, font);
}

auto BasicTextField::set_trailling_icon(const QIcon& text) -> void { }

auto BasicTextField::set_trailling_icon(const QString& code, const QString& font) -> void { }

auto BasicTextField::set_measurements(const Measurements& measurements) noexcept -> void {
    pimpl->set_measurements(measurements);
}

auto BasicTextField::resizeEvent(QResizeEvent* event) -> void {
    //
    QLineEdit::resizeEvent(event);
}

auto BasicTextField::enterEvent(qt::EnterEvent* event) -> void {
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
    pimpl->focus_out(event);
    QLineEdit::focusOutEvent(event);
}

using namespace creeper;

auto FilledTextField::paintEvent(QPaintEvent* event) -> void {
    pimpl->paint_filled(event);
    QLineEdit::paintEvent(event);
}

auto OutlinedTextField::paintEvent(QPaintEvent* event) -> void {
    pimpl->paint_outlined(event);
    QLineEdit::paintEvent(event);
}
