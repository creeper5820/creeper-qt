cmake_minimum_required(VERSION 3.22)

project(video-capture VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE "Release")
add_compile_options(-Os -O3)

find_package(spdlog REQUIRED)
find_package(OpenCV REQUIRED)

find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(yaml-cpp REQUIRED)
find_package(creeper-qt REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB_RECURSE PROJECT_CC CONFIGURE_DEPENDS "src/*.cc")
file(GLOB_RECURSE PROJECT_HH CONFIGURE_DEPENDS "src/*.hh")
file(GLOB_RECURSE PROJECT_RC CONFIGURE_DEPENDS "*.qrc")
include_directories(src)

add_executable(
  ${PROJECT_NAME}
  ${PROJECT_CC}
  ${PROJECT_HH}
  ${PROJECT_RC}
)

target_link_libraries(${PROJECT_NAME}
  Qt5::Widgets yaml-cpp
  creeper-qt::creeper-qt
  spdlog::spdlog
  ${OpenCV_LIBS}
)
