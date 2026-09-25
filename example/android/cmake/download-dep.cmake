# 安装 Qt for Android 与图标字体（由 download-dep target 调用）

include("${CMAKE_CURRENT_LIST_DIR}/tool-env.cmake")

# 以模块方式调用 aqt（${AQT_ENV}/bin/python3 -m aqt）
set(python "${AQT_ENV}/bin/python3")
set(aqt "${python}" -m aqt)

execute_process(COMMAND ${aqt} version RESULT_VARIABLE _aqt_status OUTPUT_QUIET ERROR_QUIET)
if(NOT _aqt_status EQUAL 0)
    execute_process(COMMAND python3 -m venv --clear "${AQT_ENV}" COMMAND_ERROR_IS_FATAL ANY)
    execute_process(COMMAND "${python}" -m pip install --quiet --upgrade pip aqtinstall
        COMMAND_ERROR_IS_FATAL ANY)
endif()

# 安装 linux 宿主对应的一份 Android Qt
execute_process(
    COMMAND ${aqt} install-qt linux android "${QT_VERSION}" "${QT_ANDROID_DIR}"
        --outputdir "${QT_ROOT}"
    COMMAND_ERROR_IS_FATAL ANY)

set(_android_dir "${QT_ROOT}/${QT_VERSION}/${QT_ANDROID_DIR}")
if(NOT EXISTS "${_android_dir}")
    message(FATAL_ERROR "Qt for Android 安装失败，目录不存在：${_android_dir}")
endif()

# 交叉编译需要宿主 Qt（QT_HOST_PATH）
set(_desktop_dir "${QT_ROOT}/${QT_VERSION}/gcc_64")
if(NOT EXISTS "${_desktop_dir}")
    execute_process(
        COMMAND ${aqt} install-qt linux desktop "${QT_VERSION}" linux_gcc_64
            --outputdir "${QT_ROOT}"
        COMMAND_ERROR_IS_FATAL ANY)
endif()

# 图标字体：Material Icons Round / Outlined，供应用资源打包
set(_font_base_url
    "https://raw.githubusercontent.com/google/material-design-icons/master/font")

file(MAKE_DIRECTORY "${FONT_ROOT}")

foreach(_font_name IN ITEMS MaterialIconsRound-Regular MaterialIconsOutlined-Regular)
    set(_font_url "${_font_base_url}/${_font_name}.otf")
    set(_font_file "${FONT_ROOT}/${_font_name}.otf")

    if(NOT EXISTS "${_font_file}")
        file(DOWNLOAD "${_font_url}" "${_font_file}" STATUS _font_status)
        list(GET _font_status 0 _font_status_code)
        if(NOT _font_status_code EQUAL 0)
            file(REMOVE "${_font_file}")
            list(GET _font_status 1 _font_status_message)
            message(FATAL_ERROR
                "下载图标字体失败（${_font_status_code}）：${_font_status_message}\n${_font_url}")
        endif()
    endif()
endforeach()

message(STATUS "Qt 安装完成：${_android_dir}")
message(STATUS "图标字体就绪：${FONT_ROOT}")
