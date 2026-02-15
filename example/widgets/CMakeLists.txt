cmake_minimum_required(VERSION 3.28)
project(widgets LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

if(NOT TARGET creeper-qt::widgets)
    include(FetchContent)
    FetchContent_Declare(
        creeper-qt
        GIT_REPOSITORY https://github.com/creeper5820/creeper-qt.git
        GIT_TAG main
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(creeper-qt)
endif()

file(
    GLOB_RECURSE APP_SOURCE
    CONFIGURE_DEPENDS "components/*.cc" "main.cc"
)
add_executable(${PROJECT_NAME} ${APP_SOURCE})
target_include_directories(
    ${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    Qt6::Widgets
    Qt6::Network
    creeper-qt::widgets
)
