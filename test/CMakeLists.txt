cmake_minimum_required(VERSION 3.20)
project(creeper-qt-test)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE "Release")
add_compile_options(-Os -O2)

set(CMAKE_AUTOMOC ON)

set(QT_VERSION Qt6)
find_package(${QT_VERSION} REQUIRED COMPONENTS Widgets Network)
find_package(Eigen3 REQUIRED)

# For #include "creeper-qt/..."
include_directories(..)

enable_testing()

add_executable(
    mini-bus
    utility/mini-bus.cc
)
target_link_libraries(
    mini-bus
    ${QT_VERSION}::Widgets
)
add_test(NAME mini-bus COMMAND mini-bus)
