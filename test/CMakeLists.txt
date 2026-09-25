cmake_minimum_required(VERSION 3.22)
project(creeper-qt-test LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

set(QT_VERSION Qt6)
find_package(${QT_VERSION} REQUIRED COMPONENTS Widgets Network)
find_package(Eigen3 REQUIRED)

enable_testing()

add_executable(
    mini-bus
    utility/mini-bus.cc
)
set_target_properties(
    mini-bus PROPERTIES
    AUTOMOC ON
    EXPORT_COMPILE_COMMANDS ON
)
target_compile_features(mini-bus PRIVATE cxx_std_23)
target_compile_options(mini-bus PRIVATE -Os -O2)
target_include_directories(
    mini-bus PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src   # For #include <creeper-qt/...>
)
target_link_libraries(
    mini-bus
    ${QT_VERSION}::Widgets
)
add_test(NAME mini-bus COMMAND mini-bus)
