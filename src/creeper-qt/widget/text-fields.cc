#include "text-fields.impl.hh"

BasicTextField::BasicTextField()
    : pimpl { std::make_unique<Impl>(*this) } { }

BasicTextField::~BasicTextField() = default;

auto BasicTextField::loadColorScheme(const ColorScheme& scheme) -> void {
    pimpl->loadColorScheme(scheme);
}

auto BasicTextField::bindThemeManager(ThemeManager& manager) -> void {
    pimpl->bindThemeManager(manager);
}

auto BasicTextField::setLabelText(const QString& text) -> void { pimpl->setLabelText(text); }

auto BasicTextField::setHintText(const QString& text) -> void { }

auto BasicTextField::setSupportingText(const QString& text) -> void { }

auto BasicTextField::setLeadingIcon(const QIcon& text) -> void { }

auto BasicTextField::setLeadingIcon(const QString& code, const QString& font) -> void {
    pimpl->setLeadingIcon(code, font);
}

auto BasicTextField::setTrailingIcon(const QIcon& text) -> void { }

auto BasicTextField::setTrailingIcon(const QString& code, const QString& font) -> void { }

auto BasicTextField::setMeasurements(const Measurements& measurements) noexcept -> void {
    pimpl->setMeasurements(measurements);
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

auto BasicTextField::mousePressEvent(QMouseEvent* event) -> void {
    QLineEdit::mousePressEvent(event);
    Q_EMIT pressed();
}

auto FilledTextField::paintEvent(QPaintEvent* event) -> void {
    pimpl->paint_filled(event);
    QLineEdit::paintEvent(event);
}

auto OutlinedTextField::paintEvent(QPaintEvent* event) -> void {
    pimpl->paint_outlined(event);
    QLineEdit::paintEvent(event);
}
