#include "download.hh"
#include "proxy.hh"

#include <httplib.h>

#include <fstream>
#include <print>

namespace util {

auto GetUrlTask::configure_client(httplib::Client& client) const -> void {
    client.set_connection_timeout(5);
    client.set_read_timeout(2);
    client.set_follow_location(true);
    read_proxy(client);
}

auto GetUrlTask::execute() -> void {
    if (!save_path.has_value()) {
        throw std::runtime_error { "操作取消" };
    }

    auto client = httplib::Client { base_url };
    configure_client(client);

    auto result = client.Get(path, [this](std::size_t now, std::size_t total) {
        if (progress_callback) progress_callback(now, total);
        return true;
    });

    if (result && result->status == 200) {
        auto stream = std::ofstream { *save_path, std::ios::binary };
        stream.write(result->body.data(), result->body.size());
        stream.flush();
        std::println("Download successfully");
    } else if (!result) {
        throw std::runtime_error { "Failed to download asset: Connection/DNS/Timeout Error" };
    } else {
        throw std::runtime_error { "Failed to download with http code "
            + std::to_string(result->status) };
    }
}

}
