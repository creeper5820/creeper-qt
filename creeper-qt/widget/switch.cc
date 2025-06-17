#include "widget/switch.hh"
#include "utility/animation/motion-system.hh"

using namespace creeper::_switch::internal;
using creeper::util::animation::AnimationCore;

struct Switch::Impl {
    AnimationCore animation_core;
    std::vector<std::shared_ptr<bool>> stop_tokens;

    bool checked  = false;
    bool disabled = false;

    std::shared_ptr<Eigen::Vector4d> track { std::make_shared<Eigen::Vector4d>() };
    QColor track_unchecked;
    QColor track_checked;
    QColor track_disabled;

    std::shared_ptr<Eigen::Vector4d> handle { std::make_shared<Eigen::Vector4d>() };
    QColor handle_unchecked;
    QColor handle_checked;
    QColor handle_disabled;

    std::shared_ptr<Eigen::Vector4d> outline { std::make_shared<Eigen::Vector4d>() };
    QColor outline_unchecked;
    QColor outline_checked;
    QColor outline_disabled;

    std::shared_ptr<double> position { std::make_shared<double>(0) };
    static constexpr double position_unchecked = 0.0;
    static constexpr double position_checked   = 1.0;

    explicit Impl(Switch& self)
        : animation_core([&self] { self.update(); }, 90) {
        QObject::connect(&self, &QAbstractButton::clicked, [this] { set_checked(!checked); });
    }

    void set_disabled(bool on) { }

    void set_checked(bool on) {
        if (disabled || checked == on) return;

        using Tracker4D = util::animation::FinitePidTracker<Eigen::Vector4d>;
        using Tracker1D = util::animation::FinitePidTracker<double>;

        auto stop_token = std::make_shared<bool>(false);
        double kp = 1.0, ki = 0.0, kd = 0.0, hz = 90;

        const auto track_target = checked ? track_checked : track_unchecked;
        const auto track_animation =
            Tracker4D { track, from_color(track_target), stop_token, kp, ki, kd, hz };

        const auto handle_target = checked ? handle_checked : handle_unchecked;
        const auto handle_animation =
            Tracker4D { handle, from_color(handle_target), stop_token, kp, ki, kd, hz };

        const auto outline_target = checked ? outline_checked : outline_unchecked;
        const auto outline_animation =
            Tracker4D { outline, from_color(outline_target), stop_token, kp, ki, kd, hz };

        const auto position_target = checked ? position_checked : position_unchecked;
        const auto position_animation =
            Tracker1D { position, position_target, stop_token, kp, ki, kd, hz };

        // 打断动画，将 token 设为 true 后清空，动画类持有的 token 会随着动画的结束而释放
        for (auto& stop_token : stop_tokens)
            *stop_token = true;

        stop_tokens.clear();
        stop_tokens.push_back(std::move(stop_token));

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
