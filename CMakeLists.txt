cmake_minimum_required(VERSION 3.17...3.22)

project(creeper-qt VERSION 1.0.0 LANGUAGES CXX)

# 选择是否构建示例程序
set(BUILD_EXAMPLES ON)

# 请升级g++到13以上，支持cpp23的特性
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE "Release")
add_compile_options(-Os -O3)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(yaml-cpp REQUIRED)
find_package(Eigen3 REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# File Search
file(GLOB_RECURSE PROJECT_CC "include/*.cc")
file(GLOB_RECURSE PROJECT_HH "include/*.hh")
set(PROJECT_RES "res/resource.qrc")

add_library(${PROJECT_NAME}
    SHARED
    ${PROJECT_RES}
    ${PROJECT_CC}
    ${PROJECT_HH}
    
)
add_library(${PROJECT_NAME}::widget
    ALIAS ${PROJECT_NAME}
)
target_link_libraries(${PROJECT_NAME}
    PUBLIC Qt6::Widgets yaml-cpp 
)

# Package Config
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY "include/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    FILES_MATCHING PATTERN "*.hh"
)
install(EXPORT ${PROJECT_NAME}Targets 
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake"
)

include(CMakePackageConfigHelpers)
configure_package_config_file(cmake/${PROJECT_NAME}Config.cmake.in
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake"
)

# @time: 2024-11-15
# 开发初期采用破坏式更新，姑且使用 SameMinorVersion
# 后期可以考虑使用 SameMajorVersion
write_basic_package_version_file(
    ${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMinorVersion
)

install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
              "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake"
)

set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
export(PACKAGE ${PROJECT_NAME})

add_custom_target("uninstall" COMMENT "uninstall creeper-qt library")
add_custom_command(TARGET "uninstall" POST_BUILD
    COMMENT "uninstall creeper-qt library"
    COMMAND xargs rm -vf < install_manifest.txt || echo nothing to be uninstalled
)

set(EXAMPLE_NAME widgets)
if(BUILD_EXAMPLES)
    include_directories(include)
    add_executable(${EXAMPLE_NAME}
        example/${EXAMPLE_NAME}.cc
    )
    target_link_libraries(${EXAMPLE_NAME}
        ${PROJECT_NAME}
    )
endif()

