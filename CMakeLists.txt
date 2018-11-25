cmake_minimum_required(VERSION 3.12)
project(surfaces)

set(CMAKE_CXX_STANDARD 17)

add_executable(surfaces src/main.cpp src/camera.cpp src/camera.hpp src/xgl.cpp src/xgl.hpp src/lg.cpp src/lg.hpp src/physics.cpp src/physics.hpp src/wave.cpp src/wave.hpp src/math.cpp src/math.hpp src/raft.cpp src/raft.hpp src/models.cpp src/models.hpp src/water.cpp src/water.hpp src/sun.cpp src/sun.hpp src/canvas.cpp src/canvas.hpp src/time.cpp src/time.hpp src/interframebuffer.cpp src/interframebuffer.hpp)

target_include_directories(surfaces PRIVATE glad/include stb/include)
target_link_libraries(surfaces glfw ${CMAKE_SOURCE_DIR}/glad/lib/libglad.a ${CMAKE_SOURCE_DIR}/stb/lib/libstb_image.a dl)