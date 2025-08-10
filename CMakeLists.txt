cmake_minimum_required(VERSION 3.20)
project(creeper-qt VERSION 2.0.0 LANGUAGES CXX)

set(BUILD_EXAMPLE ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE "Release")

add_compile_options(-Os -O3)

find_package(Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Eigen3 REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(.)

# 库文件收集
file(GLOB_RECURSE PROJECT_CC CONFIGURE_DEPENDS "creeper-qt/*.cc")
file(GLOB_RECURSE PROJECT_HH CONFIGURE_DEPENDS "creeper-qt/*.hh")

add_library(${PROJECT_NAME}
    SHARED
    ${PROJECT_CC}
)
add_library(${PROJECT_NAME}::widgets
    ALIAS ${PROJECT_NAME}
)
target_link_libraries(${PROJECT_NAME}
    PUBLIC
    Qt5::Widgets
    Qt5::Network
)

# 创建安装目标
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(DIRECTORY "${PROJECT_NAME}/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
    FILES_MATCHING PATTERN "*.hh"
)

# 导出 Targets 文件
install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake"
)

# 生成配置文件
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
        widgets
        example/widgets/main.cc
    )
    target_link_libraries(
        widgets
        creeper-qt
        Qt5::Widgets
    )

    add_executable(
        bluetooth
        example/bluetooth/main.cc
    )
    target_link_libraries(
        bluetooth
        creeper-qt
        Qt5::Widgets
    )

    set(APP widgets)
    add_custom_target(run
        COMMAND ${APP}
        DEPENDS ${APP}
        WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
    )
endif()
