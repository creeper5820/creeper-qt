#pragma once
#include <string>

namespace creeper::snackbar::details {

struct Message {
    std::string text;
};

class Snackbar { };

}
namespace creeper::snackbar::pro { }
namespace creeper {

using SnackbarMessage = snackbar::details::Message;

}
