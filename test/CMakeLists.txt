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

# 逐组件属性实例化测试（每个 props/*.cc 一个 OBJECT 目标，仅编译，不链接运行）
file(GLOB CREEPER_QT_PROP_TESTS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/props/*.cc")
foreach(prop_test IN LISTS CREEPER_QT_PROP_TESTS)
    get_filename_component(prop_name "${prop_test}" NAME_WE)
    add_library(
        creeper-qt-prop-${prop_name}
        OBJECT
        "${prop_test}"
    )
    set_target_properties(
        creeper-qt-prop-${prop_name} PROPERTIES
        AUTOMOC ON
        EXPORT_COMPILE_COMMANDS ON
    )
    target_compile_features(creeper-qt-prop-${prop_name} PRIVATE cxx_std_23)
    target_include_directories(
        creeper-qt-prop-${prop_name} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
        ${CMAKE_CURRENT_SOURCE_DIR}/props
    )
    target_link_libraries(
        creeper-qt-prop-${prop_name}
        ${QT_VERSION}::Widgets
        Eigen3::Eigen
    )
endforeach()
