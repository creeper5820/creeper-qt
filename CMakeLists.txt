cmake_minimum_required(VERSION 3.20)
project(creeper-qt VERSION 2.0.0 LANGUAGES CXX)

set(BUILD_EXAMPLE ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE "Release")

add_compile_options(-Os -O3)

find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(Eigen3 REQUIRED)
find_package(yaml-cpp REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(creeper-qt)

# 库文件收集
file(GLOB_RECURSE PROJECT_CC CONFIGURE_DEPENDS "creeper-qt/*.cc")
file(GLOB_RECURSE PROJECT_HH CONFIGURE_DEPENDS "creeper-qt/*.hh")

add_library(${PROJECT_NAME} SHARED
    ${PROJECT_CC}
    ${PROJECT_HH}
)
add_library(${PROJECT_NAME}::widgets
    ALIAS ${PROJECT_NAME}
)
target_link_libraries(${PROJECT_NAME}
    PUBLIC Qt5::Widgets
)

# Package Config
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY "creeper-qt/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/creeper-qt/"
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

add_custom_target("uninstall" COMMENT "uninstall ${PROJECT_NAME} library")
add_custom_command(TARGET "uninstall" POST_BUILD
    COMMENT "uninstall library ${PROJECT_NAME}"
    COMMAND xargs rm -vf < install_manifest.txt
    || echo "nothing to be uninstalled"
)

# 示例应用构建
if(BUILD_EXAMPLE)
    add_executable(
        app
        example/widgets/main.cc
    )
    target_link_libraries(
        app
        creeper-qt
        Qt5::Widgets
    )
endif()
