# 安装 Android SDK / NDK（由 download-sdk target 调用）

include("${CMAKE_CURRENT_LIST_DIR}/tool-env.cmake")

set(android_cli "${SDK_ROOT}/cmdline-tools/latest/bin/android")

if(NOT EXISTS "${android_cli}")
    message(STATUS "下载 Android command line tools ${CMDLINE_TOOLS_VERSION}")
    set(_zip "${SDK_ROOT}/cmdline-tools.zip")
    file(DOWNLOAD
        "https://dl.google.com/android/repository/commandlinetools-linux-${CMDLINE_TOOLS_VERSION}_latest.zip"
        "${_zip}"
        SHOW_PROGRESS
        TLS_VERIFY ON)
    file(MAKE_DIRECTORY "${SDK_ROOT}/cmdline-tools")
    execute_process(COMMAND "${CMAKE_COMMAND}" -E tar xf "${_zip}"
        WORKING_DIRECTORY "${SDK_ROOT}/cmdline-tools" COMMAND_ERROR_IS_FATAL ANY)
    file(RENAME "${SDK_ROOT}/cmdline-tools/cmdline-tools" "${SDK_ROOT}/cmdline-tools/latest")
    file(REMOVE "${_zip}")
endif()

# 许可接受：新 CLI 安装时会补齐 ${SDK_ROOT}/licenses/android-sdk-license，
# 这里预写该文件，保证 Gradle/Qt 部署即使未调用 CLI 也视为已接受许可。
file(MAKE_DIRECTORY "${SDK_ROOT}/licenses")
file(WRITE "${SDK_ROOT}/licenses/android-sdk-license"
"8933bad161af4178b1185d1a37fbf41ea5269c55
d56f5187479451eabf01fb78af6dfcb131a6481e
24333f8a63b6825ea9c5514f83c2829b004d1fee
")

# 基础 SDK 组件：使用同目录下的 CLI（android sdk install）。
# 该 CLI 走系统代理环境变量，无需额外的代理参数。
# 该 CLI 是先自解压再 exec 真实程序的 launcher，需经 shell 中转启动。
execute_process(
    COMMAND sh -c "exec \"$0\" --no-metrics --sdk=\"$1\" sdk install \"$2\" \"$3\" \"$4\""
        "${android_cli}" "${SDK_ROOT}"
        platform-tools
        "platforms;android-${ANDROID_API}"
        "build-tools;${ANDROID_BUILD_TOOLS}"
    COMMAND_ERROR_IS_FATAL ANY)

# 逐个校验产物是否真正落盘
set(_missing "")
if(NOT EXISTS "${SDK_ROOT}/platform-tools")
    list(APPEND _missing "${SDK_ROOT}/platform-tools")
endif()
if(NOT EXISTS "${SDK_ROOT}/build-tools/${ANDROID_BUILD_TOOLS}/apksigner")
    list(APPEND _missing "${SDK_ROOT}/build-tools/${ANDROID_BUILD_TOOLS}/apksigner")
endif()
file(GLOB _platform_dirs "${SDK_ROOT}/platforms/*")
if(NOT _platform_dirs)
    list(APPEND _missing "${SDK_ROOT}/platforms/<任意平台>")
endif()
if(_missing)
    string(REPLACE ";" "\n  " _missing_text "${_missing}")
    message(FATAL_ERROR
        "Android SDK 组件安装不完整，缺少：\n  ${_missing_text}\n"
        "如需手工排查，可执行：\n"
        "  \"${android_cli}\" --no-metrics --sdk=\"${SDK_ROOT}\" sdk install "
        "platform-tools \"platforms;android-${ANDROID_API}\" "
        "\"build-tools;${ANDROID_BUILD_TOOLS}\"")
endif()

# NDK 直接下载官方归档再解压安装
set(_ndk_toolchain
    "${SDK_ROOT}/ndk/${ANDROID_NDK_VERSION}/build/cmake/android.toolchain.cmake")

if(EXISTS "${_ndk_toolchain}")
    message(STATUS "Android NDK ${ANDROID_NDK_VERSION} 已存在，跳过安装")
else()
    set(_ndk_url
        "https://dl.google.com/android/repository/android-ndk-${ANDROID_NDK_RELEASE}-linux.zip")
    set(_ndk_zip "${SDK_ROOT}/android-ndk-${ANDROID_NDK_RELEASE}.zip")
    set(_ndk_extract_dir "${SDK_ROOT}/ndk-extract")

    message(STATUS "下载 Android NDK ${ANDROID_NDK_VERSION}（${_ndk_url}）")
    file(DOWNLOAD
        "${_ndk_url}"
        "${_ndk_zip}"
        SHOW_PROGRESS
        TLS_VERIFY ON
        STATUS _ndk_download_status)
    list(GET _ndk_download_status 0 _ndk_download_code)
    if(NOT _ndk_download_code EQUAL 0)
        list(GET _ndk_download_status 1 _ndk_download_message)
        file(REMOVE "${_ndk_zip}")
        message(FATAL_ERROR
            "Android NDK 下载失败：${_ndk_url}\n"
            "状态码：${_ndk_download_code}\n"
            "状态信息：${_ndk_download_message}")
    endif()

    message(STATUS "解压 Android NDK 到 ${_ndk_extract_dir}")
    file(MAKE_DIRECTORY "${_ndk_extract_dir}")
    execute_process(COMMAND "${CMAKE_COMMAND}" -E tar xf "${_ndk_zip}"
        WORKING_DIRECTORY "${_ndk_extract_dir}" COMMAND_ERROR_IS_FATAL ANY)

    file(MAKE_DIRECTORY "${SDK_ROOT}/ndk")
    file(RENAME
        "${_ndk_extract_dir}/android-ndk-${ANDROID_NDK_RELEASE}"
        "${SDK_ROOT}/ndk/${ANDROID_NDK_VERSION}")

    file(REMOVE "${_ndk_zip}")
    file(REMOVE_RECURSE "${_ndk_extract_dir}")

    if(NOT EXISTS "${_ndk_toolchain}")
        message(FATAL_ERROR
            "Android NDK ${ANDROID_NDK_VERSION} 安装不完整：${_ndk_toolchain} 不存在。\n"
            "如需手工排查，可执行：\n"
            "  curl -L -o /tmp/${ANDROID_NDK_RELEASE}.zip \"${_ndk_url}\"\n"
            "  cmake -E tar xf /tmp/${ANDROID_NDK_RELEASE}.zip\n"
            "  mv android-ndk-${ANDROID_NDK_RELEASE} \"${SDK_ROOT}/ndk/${ANDROID_NDK_VERSION}\"")
    endif()
endif()

message(STATUS "SDK 安装完成：${SDK_ROOT}")
