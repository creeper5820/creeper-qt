# 收集公共头文件并生成聚合头文件
file(
    GLOB_RECURSE CREEPER_QT_PUBLIC_HEADERS
    CONFIGURE_DEPENDS
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "src/${PROJECT_NAME}/*.hh"
)

# 排除实现头文件与聚合头文件本身
list(FILTER CREEPER_QT_PUBLIC_HEADERS EXCLUDE REGEX "\\.impl\\.hh$")
list(FILTER CREEPER_QT_PUBLIC_HEADERS EXCLUDE REGEX "^${PROJECT_NAME}/${PROJECT_NAME}\\.hh$")
list(SORT CREEPER_QT_PUBLIC_HEADERS)

set(CREEPER_QT_UMBRELLA_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/src/${PROJECT_NAME}/${PROJECT_NAME}.hh")
set(CREEPER_QT_UMBRELLA_CONTENT
"
// 本文件由 CMake 在配置阶段自动生成，请勿手动修改。
// 新增、删除或重命名 src/creeper-qt 下的 .hh 头文件后，
// 重新运行 cmake 即可自动更新此文件。

#pragma once

")

foreach(header IN LISTS CREEPER_QT_PUBLIC_HEADERS)
    string(APPEND CREEPER_QT_UMBRELLA_CONTENT "#include <${header}>\n")
endforeach()

file(WRITE "${CREEPER_QT_UMBRELLA_HEADER}" "${CREEPER_QT_UMBRELLA_CONTENT}")

# 收集 Widgets 模块源文件
file(
    GLOB_RECURSE WIDGETS_SOURCE
    CONFIGURE_DEPENDS
        "src/${PROJECT_NAME}/core/*.cc"
        "src/${PROJECT_NAME}/layout/*.cc"
        "src/${PROJECT_NAME}/utility/*.cc"
        "src/${PROJECT_NAME}/widget/*.cc"
        # 自定义信号头文件
        "src/${PROJECT_NAME}/widget/sliders.hh"
)
