#pragma once

// TODO: 尚未实现。当前仅有占位类型 Snackbar 与 Message 结构，
//       绘制、交互与 pro 属性入口待补全。
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
