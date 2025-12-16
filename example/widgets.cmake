if(BUILD_EXAMPLE)
    set(APP_NAME widgets)

    # WorkAround For cpp-httplib on Ubuntu
    find_package(zstd CONFIG REQUIRED)
    if (NOT TARGET zstd::libzstd)
        add_library(zstd::libzstd INTERFACE IMPORTED)
        set_property(TARGET zstd::libzstd PROPERTY
    INTERFACE_LINK_LIBRARIES zstd)
    endif()

    include(FetchContent)
    FetchContent_Declare(
        stdexec
        GIT_REPOSITORY https://github.com/NVIDIA/stdexec.git
        GIT_TAG        main
    )
    FetchContent_MakeAvailable(stdexec)

    FetchContent_Declare(
        httplib
        GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
        GIT_TAG        v0.28.0
    )
    FetchContent_MakeAvailable(httplib)

    include_directories(${HTTPLIB_HEADER_PATH})

    file(
        GLOB_RECURSE APP_SOURCE
        CONFIGURE_DEPENDS "example/widgets/*.cc"
    )
    add_executable(
        ${APP_NAME}
        ${APP_SOURCE}
    )
    # WorkAround For std::println in Windows MinGW
    if (MINGW AND CMAKE_CXX_COMPILER_ID STREQUAL GNU)
        target_link_libraries(${APP_NAME} stdc++exp)
    endif()
    target_link_libraries(
        ${APP_NAME} PRIVATE
        stdexec
        httplib
        ${QT_VERSION}::Widgets
        ${QT_VERSION}::Network
        ${PROJECT_NAME}
    )
endif()
