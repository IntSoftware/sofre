
#Exmples 1 : Hello Window
add_executable(sofre_example_HelloWindow
    helloWindow.cpp
)
target_link_libraries(sofre_example_HelloWindow PRIVATE
    sofre
)

#Exmples 1-1 : Hello Window with spear-graphics-interface
if (SOFRE_ENABLE_SGI)
    add_executable(sofre_example_sgi_HelloWindow
        helloWindow_sgi.cpp
    )
    target_link_libraries(sofre_example_sgi_HelloWindow PRIVATE
        sofre
        spear::graphic_interface
    )
endif()

