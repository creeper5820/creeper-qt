#include "proxy.hh"
#include "httplib.hh" // IWYU pragma: keep

#include <cstdlib>
#include <print>
#include <string_view>

namespace util {

auto parse_proxy_from_env() -> std::optional<std::pair<std::string, int>> {
    auto proxy = std::getenv("http_proxy") ?: std::getenv("https_proxy");
    if (!proxy) return std::nullopt;

    auto url = std::string_view { proxy };
    if (auto pos = url.find("://"); pos != std::string_view::npos) url = url.substr(pos + 3);
    if (auto colon = url.find(':'); colon != std::string_view::npos) {
        return std::make_pair(
            std::string { url.substr(0, colon) }, std::stoi(std::string { url.substr(colon + 1) }));
    }
    return std::nullopt;
}

auto read_proxy(httplib::Client& client) -> void {
    if (auto proxy = parse_proxy_from_env()) {
        std::println("Using HTTP proxy: {}:{}", proxy->first, proxy->second);
        client.set_proxy(proxy->first, proxy->second);
    }
}

}
