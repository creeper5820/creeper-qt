#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ws2tcpip.h>

#ifdef __MINGW32__
typedef INT(WSAAPI* PFN_GETADDRINFOEXCANCEL)(HANDLE* CancelHandle);
static PFN_GETADDRINFOEXCANCEL func_get_addr_info_ex_cancel = nullptr;

static INT WSAAPI GetAddrInfoExCancel(LPHANDLE lpHandle) {
    if (func_get_addr_info_ex_cancel == nullptr) {
        auto* wsdll = LoadLibraryW((wchar_t*)"ws2_32.lib");
        func_get_addr_info_ex_cancel =
            (PFN_GETADDRINFOEXCANCEL)GetProcAddress(wsdll, "GetAddrInfoExCancel");
    }

    return func_get_addr_info_ex_cancel(lpHandle);
}
#endif
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#include "httplib.h"
#pragma GCC diagnostic pop

#include "download.hh"
#include "proxy.hh"

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
