cmake_minimum_required(VERSION 3.23)

project(sofre
	VERSION 0.0.1 
	LANGUAGES C CXX
	HOMEPAGE_URL "https://github.com/IntSoftware/sofre"
)

# Is sofre the main project, not a library?
string(COMPARE EQUAL "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}" SOFRE_STANDALONE)

# C++ options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(GLM_ENABLE_CXX_17 ON)

# Build options
if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Default type of build is Debug.")
endif()

# Sofre options
option(SOFRE_BUILD_EXAMPLES "Build sofre example programs" ${SOFRE_STANDALONE})
option(SOFRE_ENABLE_SGI "Integration with spear-graphic-interface" OFF)
option(SOFRE_MEASURE_RENDERTIME "Measure render time in Renderer::render" ON)


# Compiler-specific options
if (MSVC)
    add_compile_options(/utf-8 /Zc:__cplusplus)
endif()


# main sofre library
add_library(sofre)

# external/
set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/external")
file(MAKE_DIRECTORY "${EXTERNAL_DIR}")

# Fetch gdm
set(GDM_DIR "${EXTERNAL_DIR}/gdm")

# GDM options
set(GDM_USE_GLUTIL ON CACHE BOOL "" FORCE)
set(GDM_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GDM_BUILD_TESTS OFF CACHE BOOL "" FORCE)

set(GDM_WINDOW_BACKEND "glfw" CACHE STRING "" FORCE)
set(GDM_GL_LOADER "glad" CACHE STRING "" FORCE)
set(GDM_USE_GLM ON CACHE BOOL "" FORCE)

set(GDM_GLFW_VERSION "3.4.0" CACHE STRING "" FORCE)
set(GDM_GLAD_VERSION "2.0.8" CACHE STRING "" FORCE)
set(GDM_GLM_VERSION "1.0.3" CACHE STRING "" FORCE)

set(GDM_GLAD_EXTENSION
    "GL_ARB_debug_output;GL_EXT_debug_label;GL_EXT_debug_marker;GL_EXT_texture_compression_s3tc;GL_KHR_debug;GL_EXT_texture_filter_anisotropic"
    CACHE STRING "" FORCE
)

set(FETCHCONTENT_FULLY_DISCONNECTED ON CACHE BOOL "Force offline mode" FORCE)
include(FetchContent)
FetchContent_Declare(
    gdm
    GIT_REPOSITORY https://github.com/awidesky/gdm.git
    GIT_TAG debug
    GIT_SHALLOW TRUE
    GIT_SUBMODULES ""
    GIT_REMOTE_UPDATE_STRATEGY CHECKOUT
    SOURCE_DIR "${GDM_DIR}"
)
FetchContent_MakeAvailable(gdm)

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



#TODO : how to include this file?
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
      include/sofre/camera.hpp
      include/sofre/shader.hpp
      include/sofre/texture2d.hpp
      include/sofre/program.hpp
      include/sofre/object.hpp
      include/sofre/mesh.hpp
      include/sofre/math.hpp
      include/sofre/transform.hpp
      include/sofre/enums.hpp
      include/sofre/debug.hpp
)

target_sources(sofre
  PRIVATE
    src/os_detect.hpp
    src/core.hpp
    src/sofre.cpp
    src/log.cpp
    src/engine.cpp
    src/renderer.cpp
    src/scene.cpp
    src/texture2d.cpp
    src/mesh.cpp
    src/transform.cpp
    src/object.cpp
    src/camera.cpp
    src/enums_func.hpp
    src/shader.cpp
    src/program.cpp
    src/debug.cpp
    src/gl_debug.hpp
    src/gl_debug.cpp
    src/sgi_impl.cpp

    src/tiny_obj_loader.h
    src/stb_image.h
)

target_include_directories(sofre PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}/config_files
)

target_compile_definitions(sofre PUBLIC
    $<$<BOOL:${SOFRE_ENABLE_SGI}>:SOFRE_ENABLE_SGI>

    $<$<BOOL:${SOFRE_MEASURE_RENDERTIME}>:SOFRE_MEASURE_RENDERTIME=1>
    $<$<NOT:$<BOOL:${SOFRE_MEASURE_RENDERTIME}>>:SOFRE_MEASURE_RENDERTIME=0>
)
# includes everything of gdm::deps, except for glm and compile time definitions
target_link_libraries(sofre
    PRIVATE
        gdm::opengl
        gdm::window
        gdm::loader

    PUBLIC
        gdm::defs
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
