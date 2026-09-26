cmake_minimum_required(VERSION 3.22)
project(creeper-qt VERSION 2.0.0 LANGUAGES CXX)

option(BUILD_EXAMPLE "Build the example applications" ON)

set(QT_VERSION Qt6)
find_package(${QT_VERSION} REQUIRED COMPONENTS Widgets Network)
find_package(Eigen3 REQUIRED)

include(cmake/collect.cmake)

# Widgets
add_library(${PROJECT_NAME}-widgets ${WIDGETS_SOURCE})
set_target_properties(
    ${PROJECT_NAME}-widgets PROPERTIES
    EXPORT_NAME widgets
    AUTOMOC ON
    EXPORT_COMPILE_COMMANDS ON
)
target_compile_features(${PROJECT_NAME}-widgets PUBLIC cxx_std_23)
target_compile_options(${PROJECT_NAME}-widgets PRIVATE -Os -O2)
target_include_directories(
    ${PROJECT_NAME}-widgets PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(
    ${PROJECT_NAME}-widgets
    PUBLIC
        ${QT_VERSION}::Widgets
        Eigen3::Eigen
    PRIVATE
        ${QT_VERSION}::Network
)
target_compile_definitions(${PROJECT_NAME}-widgets PUBLIC QT_NO_KEYWORDS)
add_library(${PROJECT_NAME}::widgets ALIAS ${PROJECT_NAME}-widgets)

# All Modules
add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(
    ${PROJECT_NAME} INTERFACE
    ${PROJECT_NAME}-widgets
)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

if(PROJECT_IS_TOP_LEVEL)
    include(cmake/install.cmake)
    include(cmake/package.cmake)

    if(BUILD_EXAMPLE)
        add_subdirectory(example/desktop)
    endif()
endif()
