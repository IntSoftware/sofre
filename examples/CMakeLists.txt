cmake_minimum_required(VERSION 3.16)

set(SOFRE_EXAMPLE_ASSET_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/example_config.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/example_config.hpp
    @ONLY
)

#Exmples 1 : Hello Window
add_executable(sofre_example_1_HelloWindow
    01_helloWindow.cpp
)
target_link_libraries(sofre_example_1_HelloWindow PRIVATE
    sofre
)


#Exmples 1-1 : Hello Window with spear-graphics-interface
if (SOFRE_ENABLE_SGI)
    add_executable(sofre_example_1_1_sgi_HelloWindow
        01_1_helloWindow_sgi.cpp
    )
    target_link_libraries(sofre_example_1_1_sgi_HelloWindow PRIVATE
        sofre
        spear::graphic_interface
    )
endif()


#Exmples 2 : background color change
add_executable(sofre_example_2_BackgroundColorChange
    02_backgroundColorChange.cpp
)
target_link_libraries(sofre_example_2_BackgroundColorChange PRIVATE
    sofre
)


#Exmples 3 : tiny model transform
add_executable(sofre_example_3_TransformModel
    03_transform.cpp
)
target_link_libraries(sofre_example_3_TransformModel PRIVATE
    sofre
)


#Exmples 4 : rotating cube
add_executable(sofre_example_4_RotatingCube
    04_rotatingCube.cpp
)
target_include_directories(sofre_example_4_RotatingCube
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)
target_link_libraries(sofre_example_4_RotatingCube PRIVATE
    sofre
    glm::glm
)