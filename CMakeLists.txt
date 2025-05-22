cmake_minimum_required(VERSION 3.20)
project(creeper-qt VERSION 1.0.0 LANGUAGES CXX)

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

add_executable(
    widgets
    example/widgets/main.cc
)
target_link_libraries(
    widgets
    Qt5::Widgets
    yaml-cpp
)
