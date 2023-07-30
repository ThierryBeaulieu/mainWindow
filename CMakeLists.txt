cmake_minimum_required(VERSION 3.16)

project(mainWindow VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "/Users/thierry/qt6-debug-build/" ${CMAKE_PREFIX_PATH})

#
# In case that there's issues finding the library
#
# cmake -DCMAKE_PREFIX_PATH="/path/to/qt6base-build/" /path/to/imageProc/
#

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

add_subdirectory(src/app)





