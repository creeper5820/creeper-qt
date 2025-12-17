#pragma once

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
