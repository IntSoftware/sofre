cmake_minimum_required(VERSION 3.23)

project(sofre
	VERSION 0.0.1 
	LANGUAGES C CXX
	HOMEPAGE_URL "https://github.com/IntSoftware/sofre"
)

# Is sofre the main project, not a library?
string(COMPARE EQUAL "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}" SOFRE_STANDALONE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Default type of build is Release.")
endif()

option(SOFRE_BUILD_EXAMPLES "Build sofre example programs" ${SOFRE_STANDALONE})
option(SOFRE_ENABLE_SGI "Integration with spear-graphic-interface" OFF)


# main sofre library
add_library(sofre)

# For dependencies
include(cmake/Dependencies.cmake)

# OpenGL
find_package(OpenGL REQUIRED)

# GLFW
set(SOFRE_GLFW_VERSION 3.4.0)
set(SOFRE_GLFW_GIT_TAG 3.4)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE )
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE )
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE )

set(GLFW_LIBRARY_TYPE STATIC CACHE STRING "GLFW library type")

use_or_fetch_package(
    NAME glfw3
    VERSION ${SOFRE_GLFW_VERSION}
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG ${SOFRE_GLFW_GIT_TAG}
    ALIAS_TARGET glfw::glfw
    CANDIDATE_TARGETS glfw glfw3
)


# glad_debug
add_library(glad_debug STATIC
    ${CMAKE_SOURCE_DIR}/external/glad_debug/src/gl.c
)
target_include_directories(glad_debug PUBLIC
    ${CMAKE_SOURCE_DIR}/external/glad_debug/include
)

# glad_release
add_library(glad_release STATIC
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

# glm
set(SOFRE_GLM_VERSION 1.0.3)
use_or_fetch_package(
    NAME glm
    VERSION ${SOFRE_GLM_VERSION}
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG ${SOFRE_GLM_VERSION}
)

# spear_graphic_interface
if (SOFRE_ENABLE_SGI)
	set(SOFRE_SGI_VERSION 0.0.1)
	use_or_fetch_package(
		NAME spear_graphic_interface
		VERSION ${SOFRE_SGI_VERSION}
		GIT_REPOSITORY https://github.com/IntSoftware/spear-graphic-interface
		GIT_TAG ${SOFRE_SGI_VERSION}
	)

    target_link_libraries(sofre PRIVATE spear::graphic_interface)
endif()



configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sofre_config.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/config_files/sofre_config.hpp
    @ONLY
)


# Target configuration for sofre
target_sources(sofre
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS include
    FILES
      include/sofre/sofre.hpp
      include/sofre/log.hpp
      include/sofre/window.hpp
      include/sofre/engine.hpp
      include/sofre/renderer.hpp
      include/sofre/scene.hpp
      include/sofre/shader.hpp
      include/sofre/program.hpp
      include/sofre/object.hpp
      include/sofre/mesh.hpp
      include/sofre/transform.hpp
      include/sofre/enums.hpp
)

target_sources(sofre
  PRIVATE
    src/core.hpp
    src/sofre.cpp
    src/engine.cpp
    src/renderer.cpp
    src/scene.cpp
    src/mesh.cpp
    src/transform.cpp
    src/object.cpp
    src/enums_func.hpp
    src/shader.cpp
    src/program.cpp
    src/gl_debug.hpp
    src/gl_debug.cpp
    src/sgi_impl.cpp

    src/tiny_obj_loader.h
)

target_include_directories(sofre PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}/config_files
)

target_compile_definitions(sofre PRIVATE
    $<$<CONFIG:Debug>:SOFRE_DEBUG=1>
    $<$<NOT:$<CONFIG:Debug>>:SOFRE_DEBUG=0>
 
	$<$<BOOL:${SOFRE_ENABLE_SGI}>:SOFRE_ENABLE_SGI>
)


target_link_libraries(sofre PRIVATE
	glad
    glfw::glfw
    OpenGL::GL
    glm::glm
)


# Examples
if (SOFRE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()


if (${CMAKE_GENERATOR} MATCHES "Xcode" )
	# maybe current MacOS version is lower than Xcode default deployment target version,
	# which is always the latest MacOS version.
	execute_process(COMMAND /usr/bin/sw_vers -productVersion
		OUTPUT_VARIABLE MACOS_PRODUCT_VERSION
		OUTPUT_STRIP_TRAILING_WHITESPACE)
	set(CMAKE_OSX_DEPLOYMENT_TARGET "${MACOS_PRODUCT_VERSION}")
endif()
