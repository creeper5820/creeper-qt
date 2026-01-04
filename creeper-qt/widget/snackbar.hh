#pragma once
#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include "creeper-qt/utility/wrapper/widget.hh"

#include <qwidget.h>

#include <chrono>
#include <optional>
#include <string>

namespace creeper::snackbar::details {

struct Message {
    std::string message;
    std::optional<std::string> action_label { std::nullopt };
    bool with_dismiss_action { false };
    std::chrono::milliseconds duration { std::chrono::seconds { 4 } };
};

class Snackbar : public QWidget {
    CREEPER_PIMPL_DEFINITION(Snackbar)

public:
    struct ColorSpecs {
        struct Tokens {
            QColor label_text;
            QColor state_layer;
            QColor icon;
        };
        Tokens hovered;
        Tokens focused;
        Tokens pressed; // ripple

        QColor container;
        QColor supporting_text;
    };
    struct Measurements {
        int container_height = 48; // For single line
        int text_padding     = 16;
        int action_padding   = 8;
        int button_padding   = 12;
        int button_size      = 24;
    };

    auto receive(const Message& message) noexcept -> void;

    auto set_color_scheme(const ColorScheme&) noexcept -> void;

    auto load_theme_manager(ThemeManager&) noexcept -> void;
};

}
namespace creeper::snackbar::pro { }
namespace creeper {

using SnackbarMessage = snackbar::details::Message;

}
namespace creeper::widget::pro {

struct CreateSnackbar : Token {
    ThemeManager& manager;

    explicit CreateSnackbar(ThemeManager& manager) noexcept
        : manager { manager } { }
};

}
