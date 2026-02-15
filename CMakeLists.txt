cmake_minimum_required(VERSION 3.28)
project(creeper-qt VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE "Release")

set(BUILD_EXAMPLE true)
set(CMAKE_AUTOMOC ON)

add_compile_options(-Os -O2)

set(QT_VERSION Qt6)
find_package(${QT_VERSION} REQUIRED COMPONENTS Widgets Network)
find_package(Eigen3 REQUIRED)

# 库文件收集
file(
    GLOB_RECURSE PROJECT_SOURCE
    CONFIGURE_DEPENDS
        # Project source
        "src/creeper-qt/*.cc"
        # Custom signals
        "src/creeper-qt/widget/sliders.hh"
)
add_library(${PROJECT_NAME} ${PROJECT_SOURCE})
target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
    ${QT_VERSION}::Widgets
    PRIVATE
    ${QT_VERSION}::Network
)

include(cmake/install.cmake)
include(cmake/package.cmake)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(example/widgets.cmake)
endif()
