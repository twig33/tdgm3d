# Specify the minimum version for CMake

cmake_minimum_required(VERSION 3.7)

# Project's name

project(tdgm3d)

# Set the output folder where your program will be created

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
#set(opengl_PATH "opengl path" CACHE STRING "")
#SET(opengl_IMAGELIB "${opengl_PATH}/lib/libopengl_image.dll.a")
# The following folder will be included

include_directories("${PROJECT_SOURCE_DIR}")



add_executable(tdgm3d ${PROJECT_SOURCE_DIR}/main.cpp ${PROJECT_SOURCE_DIR}/graphics.cpp ${PROJECT_SOURCE_DIR}/transform.cpp ${PROJECT_SOURCE_DIR}/collision2d.cpp ${PROJECT_SOURCE_DIR}/gameobject.cpp ${PROJECT_SOURCE_DIR}/gamestate.cpp ${PROJECT_SOURCE_DIR}/rendergroup.cpp ${PROJECT_SOURCE_DIR}/renderobject.cpp ${PROJECT_SOURCE_DIR}/shader.cpp ${PROJECT_SOURCE_DIR}/input.cpp ${PROJECT_SOURCE_DIR}/stb_image_implementation)

target_link_libraries(tdgm3d glfw GL GLEW)
