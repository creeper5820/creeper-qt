#pragma once
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include "creeper-qt/widget/widget.hh"
#include <string>

namespace creeper::snackbar::details {

class Snackbar : public creeper::Widget {
    CREEPER_PIMPL_DEFINITION(Snackbar)

public:
    struct Message {
        std::string text;
    };

    auto receive(const Message&) noexcept -> void;
};

}
namespace creeper::snackbar::pro { }
namespace creeper {

using SnackbarMessage = snackbar::details::Snackbar::Message;

}
