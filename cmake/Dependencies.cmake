function(use_or_fetch_package)
    set(options)
    set(oneValueArgs
        NAME
        VERSION
        GIT_REPOSITORY
        GIT_TAG
        EXTERNAL_PREFIX
    )
    cmake_parse_arguments(PKG
        "${options}" "${oneValueArgs}" "" ${ARGN}
    )

    set(EXTERNAL_DIR
        ${CMAKE_SOURCE_DIR}/external/${PKG_EXTERNAL_PREFIX}-${PKG_VERSION}
    )

    set(USE_EXTERNAL FALSE)

    # 1. Check external first
    if (EXISTS ${EXTERNAL_DIR}/CMakeLists.txt)
        message(STATUS
            "Using external ${PKG_NAME} ${PKG_VERSION}"
        )
        add_subdirectory(${EXTERNAL_DIR})
        return()
    endif()

    # 2. Check installed package
    find_package(${PKG_NAME} QUIET CONFIG)

    if (${PKG_NAME}_FOUND)
        if (${PKG_NAME}_VERSION VERSION_EQUAL PKG_VERSION)
            message(STATUS
                "Found installed ${PKG_NAME} ${${PKG_NAME}_VERSION}"
            )
            return()
        else()
            message(STATUS
                "Installed ${PKG_NAME} version "
                "${${PKG_NAME}_VERSION} does not match "
                "required ${PKG_VERSION}"
            )
        endif()
    endif()

    # 3. FetchContent
    message(STATUS "Fetching ${PKG_NAME} ${PKG_VERSION}")

    include(FetchContent)

    FetchContent_Declare(
        ${PKG_NAME}
        GIT_REPOSITORY ${PKG_GIT_REPOSITORY}
        GIT_TAG        ${PKG_GIT_TAG}
        SOURCE_DIR     ${EXTERNAL_DIR}
    )

    FetchContent_MakeAvailable(${PKG_NAME})
endfunction()
