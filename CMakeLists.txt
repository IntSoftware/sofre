cmake_minimum_required(VERSION 3.23)

project(sofre LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
        "Default type of build is Release." FORCE)
endif()


option(DOWNLOAD_GLFW "Download GLFW if not found in external/glfw" OFF)

# OpenGL
find_package(OpenGL REQUIRED)

# GLFW
set(GLFW_SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/glfw)
set(GLFW_TARGET glfw)
set(GLFW_LIBRARY_TYPE STATIC CACHE STRING "GLFW library type")

if(EXISTS ${GLFW_SOURCE_DIR}/CMakeLists.txt)
    add_subdirectory(${GLFW_SOURCE_DIR})

elseif(DOWNLOAD_GLFW)
    include(FetchContent)

    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG latest
        SOURCE_DIR ${GLFW_SOURCE_DIR}
    )
    FetchContent_MakeAvailable(glfw)
else()
    find_package(glfw3 CONFIG REQUIRED)

    if(TARGET glfw)
        set(GLFW_TARGET glfw)
    elseif(TARGET glfw3)
        set(GLFW_TARGET glfw3)
    else()
        message(FATAL_ERROR "GLFW target not found")
    endif()
endif()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE )
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE )
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE )


# glad_debug
add_library(glad_debug OBJECT
    ${CMAKE_SOURCE_DIR}/external/glad_debug/src/gl.c
)
target_include_directories(glad_debug PUBLIC
    ${CMAKE_SOURCE_DIR}/external/glad_debug/include
)

# glad_release
add_library(glad_release OBJECT
    ${CMAKE_SOURCE_DIR}/external/glad_release/src/gl.c
)
target_include_directories(glad_release PUBLIC
    ${CMAKE_SOURCE_DIR}/external/glad_release/include
)

# glad interface.
# If glad_debug/glad_release is directly linked, include directory does not work.
add_library(glad INTERFACE)
target_link_libraries(glad INTERFACE
    $<$<CONFIG:Debug>:glad_debug>
    $<$<NOT:$<CONFIG:Debug>>:glad_release>
)


# main sofre library
add_library(sofre)

target_sources(sofre
  PRIVATE
    src/core.hpp
    src/sofre.cpp

  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS include
    FILES
      include/sofre/sofre.hpp
)

target_link_libraries(sofre PRIVATE
	glad
    ${GLFW_TARGET}
    OpenGL::GL
)