# 下载/运行工具的公共环境：用户级写入限定在本工程 build/ 内

set(ENV{HOME} "${HOME_ROOT}")
set(ENV{ANDROID_USER_HOME} "${HOME_ROOT}")
set(ENV{ANDROID_AVD_HOME} "${HOME_ROOT}/avd")
set(ENV{ANDROID_EMULATOR_HOME} "${HOME_ROOT}/emulator")
set(ENV{ANDROID_PREFS_ROOT} "${HOME_ROOT}")
set(ENV{ANDROID_SDK_ROOT} "${SDK_ROOT}")
set(ENV{PIP_CACHE_DIR} "${HOME_ROOT}/pip-cache")
set(ENV{JAVA_TOOL_OPTIONS} "-Djava.util.prefs.userRoot=${HOME_ROOT}/java-prefs")

file(MAKE_DIRECTORY
    "${HOME_ROOT}/avd" "${HOME_ROOT}/emulator" "${HOME_ROOT}/java-prefs")
