cmake_minimum_required(VERSION 3.22)
project(creeper-qt VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

set(BUILD_EXAMPLE true)
set(CMAKE_AUTOMOC ON)

add_compile_options(-Os -O2)

set(QT_VERSION Qt6)
find_package(${QT_VERSION} REQUIRED COMPONENTS Widgets Network)
find_package(Eigen3 REQUIRED)

include(cmake/collect.cmake)

# Widgets
add_library(${PROJECT_NAME}-widgets ${WIDGETS_SOURCE})
set_target_properties(${PROJECT_NAME}-widgets PROPERTIES EXPORT_NAME widgets)
target_include_directories(
    ${PROJECT_NAME}-widgets PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(
    ${PROJECT_NAME}-widgets
    PUBLIC
        ${QT_VERSION}::Widgets
    PRIVATE
        ${QT_VERSION}::Network
)
add_library(${PROJECT_NAME}::widgets ALIAS ${PROJECT_NAME}-widgets)

# All Modules
add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(
    ${PROJECT_NAME} INTERFACE
    ${PROJECT_NAME}-widgets
)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(cmake/install.cmake)
    include(cmake/package.cmake)

    if(BUILD_EXAMPLE)
        add_subdirectory(example/widgets)
    endif()
endif()
