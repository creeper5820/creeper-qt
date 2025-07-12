#include "text-fields.hh"

using namespace creeper::text_field::internal;

constexpr auto qss = R"(
QLineEdit {
    border: none;
    color: %1;
    background-color: %2;
    selection-color: %3;
    selection-background-color: %4;
}
)";

struct BasicTextField::Impl {
public:
    explicit Impl(QLineEdit& self) noexcept
        : self { self } { }

    auto set_color_scheme(const ColorScheme& scheme) -> void { }

    auto load_theme_manager(ThemeManager& manager) -> void { }

    auto paint_event(const QPaintEvent& event) -> void { }

private:
    auto sync_basic_text_style(const QColor& text, const QColor& background,
        const QColor& selection_text, const QColor& selection_background) -> void {
        const auto qss = QString { ::qss };
        self.setStyleSheet(qss.arg(text.name())
                .arg(background.name())
                .arg(selection_text.name())
                .arg(selection_background.name()));
    }

    QLineEdit& self;
};
