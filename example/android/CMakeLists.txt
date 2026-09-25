# Android 应用工程（独立构建）
# 依赖全部通过 FetchContent 获取，不引用本目录之外的任何文件。
# 非交叉编译时本工程提供工具链下载与模拟器 target。

cmake_minimum_required(VERSION 3.22)
project(android-project LANGUAGES NONE)

set(ANDROID_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(TOOLCHAIN_DIR "${ANDROID_DIR}/build")
set(REPO_OUTPUT_DIR "${ANDROID_DIR}/build/dist")

# ======================================================================
# 交叉编译：应用
# ======================================================================
if(CMAKE_SYSTEM_NAME STREQUAL "Android")
    enable_language(CXX)

    find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)

    foreach(required IN ITEMS
            ANDROID_MIN_SDK_VERSION
            ANDROID_TARGET_SDK_VERSION
            ANDROID_COMPILE_SDK_VERSION)
        if(NOT DEFINED ${required})
            message(FATAL_ERROR "${required} 未设置：请使用 cmake --preset app")
        endif()
    endforeach()

    set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

    # ---- 依赖：Eigen ----

    include(FetchContent)
    set(FETCHCONTENT_BASE_DIR "${CMAKE_BINARY_DIR}/thirdparty")
    FetchContent_Declare(
        eigen
        GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
        GIT_TAG 3.4.0
        GIT_SHALLOW TRUE
        SOURCE_DIR "${FETCHCONTENT_BASE_DIR}/eigen3"
    )
    FetchContent_MakeAvailable(eigen)

    # Eigen3 包定义，供 creeper-qt 的 find_package(Eigen3) 使用
    file(WRITE "${CMAKE_BINARY_DIR}/eigen/Eigen3Config.cmake" "
if(NOT TARGET Eigen3::Eigen)
    add_library(Eigen3::Eigen INTERFACE IMPORTED)
    set_target_properties(Eigen3::Eigen PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES \"${FETCHCONTENT_BASE_DIR}/eigen3\")
endif()
")
    set(Eigen3_DIR "${CMAKE_BINARY_DIR}/eigen" CACHE PATH "" FORCE)

    # ---- 依赖：creeper-qt ----
    FetchContent_Declare(
        creeper-qt
        GIT_REPOSITORY https://github.com/creeper5820/creeper-qt.git
        GIT_TAG ${CREEPER_QT_GIT_TAG}
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(creeper-qt)

    # ---- 应用 ----
    qt_add_executable(${PROJECT_NAME}
        src/main.cc
        src/theme.hh
    )

    set_target_properties(
        ${PROJECT_NAME} PROPERTIES
        AUTOMOC ON
        EXPORT_COMPILE_COMMANDS ON
        QT_ANDROID_PACKAGE_NAME "com.creeper5820.widgets"
        QT_ANDROID_APP_NAME "Widgets"
        QT_ANDROID_MIN_SDK_VERSION ${ANDROID_MIN_SDK_VERSION}
        QT_ANDROID_TARGET_SDK_VERSION ${ANDROID_TARGET_SDK_VERSION}
        QT_ANDROID_COMPILE_SDK_VERSION ${ANDROID_COMPILE_SDK_VERSION}
    )

    # ---- 资源：Material Icons Round / Outlined ----
    set(FONT_FILES
        "${TOOLCHAIN_DIR}/fonts/MaterialIconsRound-Regular.otf"
        "${TOOLCHAIN_DIR}/fonts/MaterialIconsOutlined-Regular.otf"
    )
    foreach(font_file IN LISTS FONT_FILES)
        if(NOT EXISTS "${font_file}")
            message(FATAL_ERROR "缺少图标字体：请先执行 cmake --workflow --preset download-dep")
        endif()
    endforeach()

    qt_add_resources(${PROJECT_NAME} "fonts"
        PREFIX "/fonts"
        BASE "${TOOLCHAIN_DIR}/fonts"
        FILES ${FONT_FILES}
    )

    target_link_libraries(
        ${PROJECT_NAME} PRIVATE
        Qt6::Widgets
        Qt6::Network
        creeper-qt::widgets
    )

    return()
endif()

# ======================================================================
# 本地：工具链下载与模拟器
# ======================================================================
project(android-tools LANGUAGES NONE)

set(SDK_ROOT "${TOOLCHAIN_DIR}/android-sdk")
set(QT_ROOT "${TOOLCHAIN_DIR}/Qt")
set(AQT_ENV "${TOOLCHAIN_DIR}/aqt-env")
set(HOME_ROOT "${TOOLCHAIN_DIR}/home")
set(FONT_ROOT "${TOOLCHAIN_DIR}/fonts")

# 系统镜像名含分号，转义后作为单个参数传递
string(REPLACE ";" "$<SEMICOLON>" ANDROID_EMU_IMAGE_ARG "${ANDROID_EMU_IMAGE}")

set(DOWNLOAD_ARGS
    -DQT_VERSION=${QT_VERSION}
    -DQT_ANDROID_DIR=${QT_ANDROID_DIR}
    -DANDROID_API=${ANDROID_API}
    -DANDROID_BUILD_TOOLS=${ANDROID_BUILD_TOOLS}
    -DANDROID_NDK_VERSION=${ANDROID_NDK_VERSION}
    -DANDROID_NDK_RELEASE=${ANDROID_NDK_RELEASE}
    -DCMDLINE_TOOLS_VERSION=${CMDLINE_TOOLS_VERSION}
    -DANDROID_EMU_IMAGE=${ANDROID_EMU_IMAGE_ARG}
    -DANDROID_EMU_NAME=${ANDROID_EMU_NAME}
    -DSDK_ROOT=${SDK_ROOT}
    -DQT_ROOT=${QT_ROOT}
    -DAQT_ENV=${AQT_ENV}
    -DHOME_ROOT=${HOME_ROOT}
    -DFONT_ROOT=${FONT_ROOT}
)

add_custom_target(download-sdk
    COMMENT "安装 Android SDK / NDK"
    COMMAND ${CMAKE_COMMAND} ${DOWNLOAD_ARGS} -P "${ANDROID_DIR}/cmake/download-sdk.cmake"
    USES_TERMINAL
    VERBATIM
)

add_custom_target(download-dep
    COMMENT "安装 Qt for Android 与图标字体"
    COMMAND ${CMAKE_COMMAND} ${DOWNLOAD_ARGS} -P "${ANDROID_DIR}/cmake/download-dep.cmake"
    USES_TERMINAL
    VERBATIM
)

add_custom_target(download-emu
    COMMENT "安装 emulator 与系统镜像"
    COMMAND ${CMAKE_COMMAND} ${DOWNLOAD_ARGS} -P "${ANDROID_DIR}/cmake/download-emu.cmake"
    USES_TERMINAL
    VERBATIM
)

add_custom_target(emulator
    COMMENT "无头启动模拟器"
    COMMAND ${CMAKE_COMMAND} ${DOWNLOAD_ARGS}
        -DANDROID_EMU_ABI=${ANDROID_EMU_ABI}
        -P "${ANDROID_DIR}/cmake/emulator.cmake"
    USES_TERMINAL
    VERBATIM
)

add_custom_target(emulator-window
    COMMENT "带窗口启动模拟器"
    COMMAND ${CMAKE_COMMAND} ${DOWNLOAD_ARGS}
        -DANDROID_EMU_ABI=${ANDROID_EMU_ABI}
        -DEMU_WINDOW=ON
        -P "${ANDROID_DIR}/cmake/emulator.cmake"
    USES_TERMINAL
    VERBATIM
)

add_custom_target(emulator-stop
    COMMENT "关闭模拟器"
    COMMAND "${SDK_ROOT}/platform-tools/adb" -s emulator-5554 shell reboot -p
    USES_TERMINAL
    VERBATIM
)

add_custom_target(package
    COMMENT "归集 APK 到 build/"
    COMMAND ${CMAKE_COMMAND}
        "-DAPP_BUILD_DIR=${ANDROID_DIR}/build/app"
        "-DOUTPUT_DIR=${REPO_OUTPUT_DIR}"
        "-DANDROID_ABI=${ANDROID_ABI}"
        -P "${ANDROID_DIR}/cmake/package-apk.cmake"
    USES_TERMINAL
    VERBATIM
)
