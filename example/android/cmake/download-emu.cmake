# 安装 emulator 与系统镜像（由 download-emu target 调用）

include("${CMAKE_CURRENT_LIST_DIR}/tool-env.cmake")

set(android_cli "${SDK_ROOT}/cmdline-tools/latest/bin/android")

# 使用同目录下的 CLI（android sdk install）。
# 该 CLI 走系统代理环境变量，无需额外的代理参数。
# 该 CLI 是先自解压再 exec 真实程序的 launcher，需经 shell 中转启动。
execute_process(
    COMMAND sh -c "exec \"$0\" --no-metrics --sdk=\"$1\" sdk install \"$2\" \"$3\""
        "${android_cli}" "${SDK_ROOT}"
        emulator
        "${ANDROID_EMU_IMAGE}"
    COMMAND_ERROR_IS_FATAL ANY)

# 逐个校验产物是否真正落盘
# 系统镜像名形如 system-images;android-36;google_apis;x86_64，
# 分号换成 / 后本身就是相对 SDK 根目录的路径。
string(REPLACE ";" "/" _emulator_image_dir "${ANDROID_EMU_IMAGE}")

set(_missing "")
if(NOT EXISTS "${SDK_ROOT}/emulator/emulator")
    list(APPEND _missing "${SDK_ROOT}/emulator/emulator")
endif()
if(NOT EXISTS "${SDK_ROOT}/${_emulator_image_dir}")
    list(APPEND _missing "${SDK_ROOT}/${_emulator_image_dir}")
endif()
if(_missing)
    string(REPLACE ";" "\n  " _missing_text "${_missing}")
    message(FATAL_ERROR
        "模拟器组件安装不完整，缺少：\n  ${_missing_text}\n"
        "如需手工排查，可执行：\n"
        "  \"${android_cli}\" --no-metrics --sdk=\"${SDK_ROOT}\" sdk install "
        "emulator \"${ANDROID_EMU_IMAGE}\"")
endif()

message(STATUS "模拟器安装完成：${SDK_ROOT}/emulator")
