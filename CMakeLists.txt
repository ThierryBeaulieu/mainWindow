cmake_minimum_required(VERSION 3.16)

project(mainWindow VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

include_directories(src/include)
include_directories(src/glm)
add_subdirectory(src/app)

configure_file(${CMAKE_SOURCE_DIR}/src/assets/awesomeface.png ${CMAKE_BINARY_DIR}/src/assets/awesomeface.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/src/assets/container.jpg ${CMAKE_BINARY_DIR}/src/assets/container.jpg COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/src/assets/wall.jpg ${CMAKE_BINARY_DIR}/src/assets/wall.jpg COPYONLY)

configure_file(${CMAKE_SOURCE_DIR}/src/shaders/shader.vs ${CMAKE_BINARY_DIR}/src/shaders/shader.vs COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/src/shaders/shader.fs ${CMAKE_BINARY_DIR}/src/shaders/shader.fs COPYONLY)





