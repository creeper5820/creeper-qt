cmake_minimum_required(VERSION 3.22)
project(widgets LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

if(PROJECT_IS_TOP_LEVEL)
    include(FetchContent)
    FetchContent_Declare(
        creeper-qt
        GIT_REPOSITORY https://github.com/creeper5820/creeper-qt.git
        GIT_TAG main
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(creeper-qt)
elseif(NOT TARGET creeper-qt::widgets)
    message(FATAL_ERROR "creeper-qt::widgets 不存在：请独立构建 example/desktop，或在仓库根目录启用 BUILD_EXAMPLE")
endif()

file(
    GLOB_RECURSE APP_COMPONENT_SOURCE
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/components/*.cc"
)
add_executable(${PROJECT_NAME}
    ${APP_COMPONENT_SOURCE}
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cc"
)
set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    AUTOMOC ON
    EXPORT_COMPILE_COMMANDS ON
)
target_include_directories(
    ${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    Qt6::Widgets
    Qt6::Network
    creeper-qt::widgets
)
