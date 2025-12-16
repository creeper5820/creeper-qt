#pragma once

#include <optional>
#include <string>

namespace httplib {
class Client;
}

namespace util {

// 从环境变量读取代理配置并应用到 httplib::Client
// 支持 http_proxy 和 https_proxy 环境变量
// 格式支持: http://host:port, https://host:port, host:port
auto read_proxy(httplib::Client& client) -> void;

// 从环境变量读取代理配置，返回 host 和 port
// 如果未设置代理，返回 std::nullopt
auto parse_proxy_from_env() -> std::optional<std::pair<std::string, int>>;

}
