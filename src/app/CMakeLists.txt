add_executable(mainWindow
    main.cpp
    shader.h
)

target_link_libraries(mainWindow OpenGL::GL glfw GLEW::GLEW)