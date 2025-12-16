if(BUILD_EXAMPLE)
    set(APP_NAME widgets)

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
    target_link_libraries(
        ${APP_NAME} PRIVATE
        stdexec
        httplib
        ${QT_VERSION}::Widgets
        ${QT_VERSION}::Network
        ${PROJECT_NAME}
    )
endif()
