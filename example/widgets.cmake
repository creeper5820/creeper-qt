if(BUILD_EXAMPLE)
    set(APP_NAME widgets)
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
        ${QT_VERSION}::Widgets
        ${QT_VERSION}::Network
        ${PROJECT_NAME}
    )
endif()
