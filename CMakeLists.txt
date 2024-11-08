cmake_minimum_required(VERSION 3.20)

project(creeper-qt-example VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE "Release")
add_compile_options(-Os -O3)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

file(GLOB_RECURSE PROJECT_CC "src/*.cc")
file(GLOB_RECURSE PROJECT_HH "src/*.hh")

qt_add_executable(
    ${PROJECT_NAME}
    ${PROJECT_CC}
    ${PROJECT_HH}
    "res/resource.qrc"
)

target_link_libraries(
    ${PROJECT_NAME} 
    PRIVATE Qt6::Widgets
)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
)