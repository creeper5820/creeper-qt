#include "creeper-qt/utility/animation/core.hh"
#include "text-fields.hh"

using namespace creeper;
using namespace creeper::text_field::internal;

using util::animation::AnimationCore;

struct BasicTextField::Impl {
public:
    explicit Impl(BasicTextField& self) noexcept
        : self { self }
        , animation_core { [&self] { self.update(); }, AnimationCore::kHz } { }

    /// @note https://m3.material.io/components/text-fields/specs
    auto set_color_scheme(const ColorScheme& scheme) -> void {

        self.color.enabled.container        = scheme.surface_container_highest;
        self.color.enabled.label_text       = scheme.on_surface_variant;
        self.color.enabled.leading_icon     = scheme.on_surface_variant;
        self.color.enabled.trailing_icon    = scheme.on_surface_variant;
        self.color.enabled.active_indicator = scheme.on_surface_variant;
        self.color.enabled.supporting_text  = scheme.on_surface_variant;
        self.color.enabled.input_text       = scheme.on_surface;
        self.color.enabled.caret            = scheme.primary;

        self.color.disabled.container = scheme.on_surface;
        self.color.disabled.container.setAlphaF(0.04);
        self.color.disabled.label_text = scheme.on_surface;
        self.color.disabled.label_text.setAlphaF(0.38);
        self.color.disabled.leading_icon = scheme.on_surface;
        self.color.disabled.leading_icon.setAlphaF(0.38);
        self.color.disabled.trailing_icon = scheme.on_surface;
        self.color.disabled.trailing_icon.setAlphaF(0.38);
        self.color.disabled.supporting_text = scheme.on_surface;
        self.color.disabled.supporting_text.setAlphaF(0.38);
        self.color.disabled.input_text = scheme.on_surface;
        self.color.disabled.input_text.setAlphaF(0.38);
        self.color.disabled.active_indicator = scheme.on_surface;
        self.color.disabled.active_indicator.setAlphaF(0.38);

        self.color.focused.label_text       = scheme.primary;
        self.color.focused.leading_icon     = scheme.on_surface_variant;
        self.color.focused.trailing_icon    = scheme.on_surface_variant;
        self.color.focused.input_text       = scheme.on_surface;
        self.color.focused.supporting_text  = scheme.on_surface_variant;
        self.color.focused.active_indicator = scheme.primary;

        self.color.error.active_indicator = scheme.error;
        self.color.error.label_text       = scheme.error;
        self.color.error.input_text       = scheme.on_surface;
        self.color.error.supporting_text  = scheme.error;
        self.color.error.leading_icon     = scheme.on_surface_variant;
        self.color.error.trailing_icon    = scheme.error;
        self.color.error.caret            = scheme.error;

        self.color.state_layer = scheme.on_surface;
        self.color.state_layer.setAlphaF(0.08);
        self.color.selection_container = scheme.primary;
        self.color.selection_container.setAlphaF(0.38);

        update_ui_state();

        sync_basic_text_style(             //
            self.color.current.input_text, //
            self.color.current.container,  //
            self.color.current.input_text, //
            self.color.selection_container //
        );
    }

    auto load_theme_manager(ThemeManager& manager) -> void {
        manager.append_handler(&self, [this](const ThemeManager& manager) { //
            set_color_scheme(manager.color_scheme());
        });
    }

    auto paint_event(const QPaintEvent& event) -> void { }

private:
    auto update_ui_state() -> void {
        self.color.current //
            = is_disable ? self.color.disabled
            : is_error   ? self.color.error
            : is_focused ? self.color.focused
                         : self.color.enabled;
    }

    auto sync_basic_text_style(const QColor& text, const QColor& container,
        const QColor& selection_text, const QColor& selection_container) -> void {

        constexpr auto to_rgba = [](const QColor& color) {
            return QStringLiteral("rgba(%1, %2, %3, %4)")
                .arg(color.red())
                .arg(color.green())
                .arg(color.blue())
                .arg(color.alpha());
        };

        constexpr auto kQLineEditStyle = R"(
            QLineEdit {
                border: none;
                color: %1;
                background-color: %2;
                selection-color: %3;
                selection-background-color: %4;
            }
        )";

        const auto qss = QString { kQLineEditStyle };
        self.setStyleSheet(qss.arg(to_rgba(text))
                .arg(to_rgba(container))
                .arg(to_rgba(selection_text))
                .arg(to_rgba(selection_container)));
    }

private:
    BasicTextField& self;
    AnimationCore animation_core;

    bool is_disable = false;
    bool is_hovered = false;
    bool is_focused = false;
    bool is_error   = false;
};
