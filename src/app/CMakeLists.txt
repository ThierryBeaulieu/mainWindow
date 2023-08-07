add_executable(mainWindow
    main.cpp
)

target_link_libraries(mainWindow OpenGL::GL glfw GLEW::GLEW SDL2::SDL2)