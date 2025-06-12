cmake_minimum_required(VERSION 3.20)
project(creeper-qt VERSION 2.0.0 LANGUAGES CXX)

set(BUILD_EXAMPLE ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE "Release")

add_compile_options(-Os -O3)

find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(Eigen3 REQUIRED)
find_package(yaml-cpp REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(creeper-qt)

# 库文件收集
file(GLOB_RECURSE PROJECT_CC CONFIGURE_DEPENDS "creeper-qt/*.cc")
file(GLOB_RECURSE PROJECT_HH CONFIGURE_DEPENDS "creeper-qt/*.hh")

add_library(${PROJECT_NAME} SHARED
    ${PROJECT_CC}
    ${PROJECT_HH}
)
target_link_libraries(${PROJECT_NAME}
    PUBLIC Qt5::Widgets
)

add_executable(
    app
    example/widgets/main.cc
)
target_link_libraries(
    app
    creeper-qt
    Qt5::Widgets
)
