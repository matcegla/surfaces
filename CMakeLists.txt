cmake_minimum_required(VERSION 3.12)
project(surfaces)

set(CMAKE_CXX_STANDARD 17)

add_executable(surfaces main.cpp)

target_include_directories(surfaces PRIVATE glad/include stb/include)
target_link_libraries(surfaces glfw ${CMAKE_SOURCE_DIR}/glad/lib/libglad.a ${CMAKE_SOURCE_DIR}/stb/lib/libstb_image.a dl)