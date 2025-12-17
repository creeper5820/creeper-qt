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
    set(FETCHCONTENT_QUIET OFF)

    FetchContent_Declare(
        stdexec
        GIT_REPOSITORY https://github.com/NVIDIA/stdexec.git
        GIT_TAG        f146a8f05cff8fc97c1985ca3dc87f6f631e85de
        GIT_PROGRESS   TRUE
    )
    set(STDEXEC_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(STDEXEC_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(stdexec)

    FetchContent_Declare(
        httplib
        GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
        GIT_TAG        v0.28.0
        GIT_SHALLOW    TRUE
        GIT_PROGRESS   TRUE
    )
    FetchContent_MakeAvailable(httplib)

    file(
        GLOB_RECURSE APP_SOURCE
        CONFIGURE_DEPENDS "example/widgets/*.cc"
    )
    add_executable(
        ${APP_NAME}
        ${APP_SOURCE}
    )
    target_link_libraries(
        ${APP_NAME} PRIVATE
        stdc++exp # WorkAround For std::println in Windows MinGW
        stdexec
        httplib::httplib
        ${QT_VERSION}::Widgets
        ${QT_VERSION}::Network
        ${PROJECT_NAME}
    )
endif()
