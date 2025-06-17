#include "widget/switch.hh"
#include "utility/animation/motion-system.hh"

using namespace creeper::_switch::internal;
using creeper::util::animation::AnimationCore;

struct Switch::Impl {
    AnimationCore animation_core;
    std::vector<std::shared_ptr<bool>> stop_tokens;

    bool checked  = false;
    bool disabled = false;

    std::shared_ptr<QColor> track { std::make_shared<QColor>() };
    std::shared_ptr<QColor> track_target { std::make_shared<QColor>() };
    QColor track_unchecked;
    QColor track_checked;
    QColor track_disabled;

    std::shared_ptr<QColor> handle { std::make_shared<QColor>() };
    std::shared_ptr<QColor> handle_target { std::make_shared<QColor>() };
    QColor handle_unchecked;
    QColor handle_checked;
    QColor handle_disabled;

    std::shared_ptr<QColor> outline { std::make_shared<QColor>() };
    std::shared_ptr<QColor> outline_target { std::make_shared<QColor>() };
    QColor outline_unchecked;
    QColor outline_checked;
    QColor outline_disabled;

    std::shared_ptr<double> position { std::make_shared<double>(0) };
    std::shared_ptr<double> position_target { std::make_shared<double>(0) };

    explicit Impl(Switch& self)
        : animation_core([&self] { self.update(); }, 90) {
        QObject::connect(&self, &QAbstractButton::clicked, [this] { set_checked(!checked); });

        using namespace util::animation;
    }

    void set_checked(bool on) {
        if (disabled || checked == on) return;
        checked = on;
    }

    void enter_event(Switch& self, const QEvent& event) { }

    void leave_event(Switch& self, const QEvent& event) { }

    void paint_event(Switch& self, const QPaintEvent& event) { }
};

Switch::Switch()
    : pimpl(std::make_unique<Impl>(*this)) { }

Switch::~Switch() = default;

void Switch::set_color_scheme(const ColorScheme&) { }
void Switch::load_theme_manager(ThemeManager&) { }

void Switch::set_checked(bool on) { pimpl->checked = on; }
bool Switch::checked() const { return pimpl->checked; }

void Switch::set_track_color_unchecked(QColor color) { pimpl->track_unchecked = color; }
void Switch::set_track_color_checked(QColor color) { pimpl->track_checked = color; }
void Switch::set_track_color_disabled(QColor color) { pimpl->track_disabled = color; }

void Switch::set_handle_color_unchecked(QColor color) { pimpl->handle_unchecked = color; }
void Switch::set_handle_color_checked(QColor color) { pimpl->handle_checked = color; }
void Switch::set_handle_color_disabled(QColor color) { pimpl->handle_disabled = color; }

void Switch::set_outline_color_unchecked(QColor color) { pimpl->outline_unchecked = color; }
void Switch::set_outline_color_checked(QColor color) { pimpl->outline_checked = color; }
void Switch::set_outline_color_disabled(QColor color) { pimpl->outline_disabled = color; }

void Switch::enterEvent(QEvent* event) {
    pimpl->enter_event(*this, *event);
    QAbstractButton::enterEvent(event);
}

void Switch::leaveEvent(QEvent* event) {
    pimpl->leave_event(*this, *event);
    QAbstractButton::leaveEvent(event);
}

void Switch::paintEvent(QPaintEvent* event) { pimpl->paint_event(*this, *event); }
