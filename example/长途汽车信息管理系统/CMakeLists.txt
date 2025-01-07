cmake_minimum_required(VERSION 3.17)

project(app)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE "Debug")
add_compile_options(-O3)
add_definitions(-DPROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(yaml-cpp REQUIRED)
find_package(creeper-qt REQUIRED)

find_path(mysql mysql REQUIRED)
find_library(mysqlpp mysqlpp CONFIG REQUIRED)

include_directories(/usr/include/mysql/ .)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB_RECURSE PROJECT_CC CONFIGURE_DEPENDS "./*.cc")
file(GLOB_RECURSE PROJECT_HH CONFIGURE_DEPENDS "./*.hh")

add_executable(${PROJECT_NAME}
    ${PROJECT_CC}
    ${PROJECT_HH}
    resource.qrc
)
target_link_libraries(${PROJECT_NAME}
    creeper-qt::creeper-qt
    Qt5::Widgets yaml-cpp
    mysqlpp mysqlclient
)
