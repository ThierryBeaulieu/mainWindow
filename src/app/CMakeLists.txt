add_executable(mainWindow
    main.cpp
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_impl_glfw_gl3.cpp
    imgui.cpp
)

target_link_libraries(mainWindow OpenGL::GL glfw GLEW::GLEW)
