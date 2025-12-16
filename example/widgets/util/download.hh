#pragma once

#include <functional>
#include <optional>
#include <string>

namespace httplib {
class Client;
}

namespace util {

// HTTP 下载任务
// 支持进度回调和代理配置
struct GetUrlTask {
    std::string base_url;
    std::string path;
    std::optional<std::string> save_path;
    std::function<void(std::size_t, std::size_t)> progress_callback;

    // 执行下载任务
    // 成功返回 true，失败抛出 std::runtime_error
    auto execute() -> void;

    // 配置客户端超时和代理
    auto configure_client(httplib::Client& client) const -> void;
};

}
