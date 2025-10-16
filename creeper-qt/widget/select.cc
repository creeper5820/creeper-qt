#include "select.impl.hh"
#include <memory>
#include "select.hh"

BasicSelect::BasicSelect()
    : pimpl {std::make_unique<Impl>(*this)} {}

BasicSelect::~BasicSelect() = default;

void BasicSelect::set_color_scheme(const ColorScheme &scheme) {
    pimpl->set_color_scheme(scheme);
}

void BasicSelect::load_theme_manager(ThemeManager &manager) {
    pimpl->load_theme_manager(manager);
}

void BasicSelect::set_label_text(const QString &text) {
    pimpl->set_label_text(text);
}

void BasicSelect::set_leading_icon(const QIcon &) {
}

void BasicSelect::set_leading_icon(const QString &code, const QString &font) {
    pimpl->set_leading_icon(code, font);
}

void BasicSelect::resizeEvent(QResizeEvent *event) {
    QComboBox::resizeEvent(event);
}

void BasicSelect::enterEvent(qt::EnterEvent *enter_event) {
    pimpl->enter_event(enter_event);
    QComboBox::enterEvent(enter_event);
}

void BasicSelect::leaveEvent(QEvent *event) {
    pimpl->leave_event(event);
    QComboBox::leaveEvent(event);
}

void BasicSelect::focusInEvent(QFocusEvent *focus_event) {
    pimpl->focus_in(focus_event);
    QComboBox::focusInEvent(focus_event);
}

void BasicSelect::focusOutEvent(QFocusEvent *event) {
    pimpl->focus_out(event);
    QComboBox::focusOutEvent(event);
}

void BasicSelect::changeEvent(QEvent *event) {
    QComboBox::changeEvent(event);
}

void BasicSelect::showPopup() {
    pimpl->show_popup();
}

void BasicSelect::hidePopup() {
    pimpl->hide_popup();
}

auto BasicSelect::set_measurements(const Measurements &measurements) noexcept -> void {
    pimpl->set_measurements(measurements);
}

void BasicSelect::setTextMargins(const QMargins &margins) {
    this->margins = margins;
}

QMargins BasicSelect::textMargins() const {
    return margins;
}


using namespace creeper;

void FilledSelect::paintEvent(QPaintEvent *event) {
    pimpl->paint_filled(event);
    // QComboBox::paintEvent(event);
}