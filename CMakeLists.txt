cmake_minimum_required(VERSION 3.20)
project(creeper-qt VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE "Release")

set(CMAKE_AUTOMOC ON)

add_compile_options(-Os -O3)

set(QT_VERSION Qt6)
find_package(${QT_VERSION} REQUIRED COMPONENTS Widgets Network)
find_package(Eigen3 REQUIRED)

# For #include "creeper-qt/..."
include_directories(.)

# 库文件收集
file(
    GLOB_RECURSE PROJECT_SOURCE
    CONFIGURE_DEPENDS
        # Project source
        "creeper-qt/*.cc"
        # Custom signals
        "creeper-qt/widget/sliders.hh"
)
add_library(
    ${PROJECT_NAME} SHARED
    ${PROJECT_SOURCE}
)
target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    ${QT_VERSION}::Widgets
    ${QT_VERSION}::Network
)

include(cmake/install.cmake)
include(cmake/package.cmake)
include(example/widgets/widgets.cmake)
