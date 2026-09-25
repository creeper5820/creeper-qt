# 容器与宿主机共用的 Android 交叉编译工具链。
# 仅引用本工程内的相对路径；版本值由 CMakePresets.json 的 base preset 提供。

# 读取 CMakePresets.json 的 base preset，使编译器探测与多 ABI 子配置也能拿到版本值
set(_presets_file "${CMAKE_CURRENT_LIST_DIR}/../CMakePresets.json")
file(READ "${_presets_file}" _presets_json)
string(JSON _preset_count LENGTH "${_presets_json}" configurePresets)
math(EXPR _preset_last "${_preset_count} - 1")
foreach(_i RANGE "${_preset_last}")
    string(JSON _preset_name GET "${_presets_json}" configurePresets ${_i} name)
    if(_preset_name STREQUAL "base")
        string(JSON _var_count LENGTH "${_presets_json}" configurePresets ${_i} cacheVariables)
        math(EXPR _var_last "${_var_count} - 1")
        foreach(_j RANGE "${_var_last}")
            string(JSON _key MEMBER "${_presets_json}" configurePresets ${_i} cacheVariables ${_j})
            string(JSON _value GET "${_presets_json}" configurePresets ${_i} cacheVariables "${_key}")
            if(NOT DEFINED ${_key} OR "${${_key}}" STREQUAL "")
                set(${_key} "${_value}" CACHE STRING "" FORCE)
            endif()
        endforeach()
    endif()
endforeach()

set(CMAKE_SYSTEM_NAME Android)
set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)

set(CMAKE_TRY_COMPILE_PLATFORM_VARIABLES
    QT_VERSION QT_ANDROID_DIR ANDROID_NDK_VERSION
    ANDROID_MIN_SDK_VERSION ANDROID_TARGET_SDK_VERSION ANDROID_COMPILE_SDK_VERSION
    CREEPER_QT_GIT_TAG)

set(_root "${CMAKE_CURRENT_LIST_DIR}/..")
set(ANDROID_SDK_ROOT "${_root}/build/android-sdk" CACHE PATH "" FORCE)
set(ANDROID_NDK_ROOT "${ANDROID_SDK_ROOT}/ndk/${ANDROID_NDK_VERSION}" CACHE PATH "" FORCE)
set(QT_HOST_PATH "${_root}/build/Qt/${QT_VERSION}/gcc_64" CACHE PATH "" FORCE)

include("${_root}/build/Qt/${QT_VERSION}/${QT_ANDROID_DIR}/lib/cmake/Qt6/qt.toolchain.cmake")
