#pragma once
#include "creeper-qt/utility/wrapper/pimpl.hh"

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
    auto receive(const Message& message) noexcept -> void;
};

}
namespace creeper::snackbar::pro { }
namespace creeper {

using SnackbarMessage = snackbar::details::Message;

}
