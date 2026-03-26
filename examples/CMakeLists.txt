cmake_minimum_required(VERSION 3.16)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/assets" SOFRE_EXAMPLE_ASSET_DIR)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/example_config.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/example_config.hpp
    @ONLY
)

#Exmples 1 : Hello Window
add_executable(sofre_example_1_HelloWindow
    01_helloWindow.cpp
)
set_target_properties(sofre_example_1_HelloWindow PROPERTIES FOLDER "SOFRE_Examples")
target_link_libraries(sofre_example_1_HelloWindow PRIVATE
    sofre
)


#Exmples 1-1 : Hello Window with spear-graphics-interface
if (SOFRE_ENABLE_SGI)
    add_executable(sofre_example_1_1_sgi_HelloWindow
        01_1_helloWindow_sgi.cpp
    )
    set_target_properties(sofre_example_1_1_sgi_HelloWindow PROPERTIES FOLDER "SOFRE_Examples")
    target_link_libraries(sofre_example_1_1_sgi_HelloWindow PRIVATE
        sofre
        spear::graphic_interface
    )
endif()


#Exmples 2 : Vertex attribute and uniform
add_executable(sofre_example_2_VertexAttribAndUniform
    02_vertexAttribAndUniform.cpp
)
set_target_properties(sofre_example_2_VertexAttribAndUniform PROPERTIES FOLDER "SOFRE_Examples")
target_link_libraries(sofre_example_2_VertexAttribAndUniform PRIVATE
    sofre
)


#Exmples 3 : 2D Camera
add_executable(sofre_example_3_Cam2D
    03_cam2D.cpp
)
set_target_properties(sofre_example_3_Cam2D PROPERTIES FOLDER "SOFRE_Examples")
target_link_libraries(sofre_example_3_Cam2D PRIVATE
    sofre
)


#Exmples 4 : rotating cube
add_executable(sofre_example_4_RotatingCube
    04_rotatingCube.cpp
)
set_target_properties(sofre_example_4_RotatingCube PROPERTIES FOLDER "SOFRE_Examples")
target_include_directories(sofre_example_4_RotatingCube
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)
target_link_libraries(sofre_example_4_RotatingCube PRIVATE
    sofre
)


#Exmples 5 : cube with essets
add_executable(sofre_example_5_TextureCube
    05_textureCube.cpp
)
set_target_properties(sofre_example_5_TextureCube PROPERTIES FOLDER "SOFRE_Examples")
target_include_directories(sofre_example_5_TextureCube
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)
target_link_libraries(sofre_example_5_TextureCube PRIVATE
    sofre
)


#Exmples 6 : many rotating cube
add_executable(sofre_example_6_ManyCubes
    06_manyCubes.cpp
)
set_target_properties(sofre_example_6_ManyCubes PROPERTIES FOLDER "SOFRE_Examples")
target_include_directories(sofre_example_6_ManyCubes
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)
target_link_libraries(sofre_example_6_ManyCubes PRIVATE
    sofre
)
