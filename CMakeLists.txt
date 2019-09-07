cmake_minimum_required(VERSION 3.12)
project(surfaces)

set(CMAKE_CXX_STANDARD 17)
set(SURFACES_SOURCES src/camera.cpp src/canvas.cpp src/debug.cpp src/inter.cpp src/lg.cpp src/main.cpp src/math.cpp src/models.cpp src/physics.cpp src/raft.cpp src/screenbuffer.cpp src/sun.cpp src/time.cpp src/water.cpp src/wave.cpp src/xgl.cpp)
set(SURFACES_HEADERS src/camera.hpp src/canvas.hpp src/debug.hpp src/inter.hpp src/lg.hpp              src/math.hpp src/models.hpp src/physics.hpp src/raft.hpp src/screenbuffer.hpp src/sun.hpp src/time.hpp src/water.hpp src/wave.hpp src/xgl.hpp)

find_program(CLANG_FORMAT_EXE NAMES "clang-format" DOC "Path to clang-format executable")
if(NOT CLANG_FORMAT_EXE)
    message(STATUS "clang-format not found")
else()
    message(STATUS "clang-format found at ${CLANG_FORMAT_EXE}")
endif()

add_executable(surfaces ${SURFACES_HEADERS} ${SURFACES_SOURCES})
if(CLANG_FORMAT_EXE)
    set(TO_FORMAT ${SURFACES_SOURCES};${SURFACES_HEADERS})
    list(TRANSFORM TO_FORMAT PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)
    add_custom_target(format COMMAND ${CLANG_FORMAT_EXE} -i ${TO_FORMAT})
endif()

target_include_directories(surfaces PRIVATE vendor/glad/include vendor/stb/include)
target_link_libraries(surfaces glfw ${CMAKE_SOURCE_DIR}/vendor/glad/lib/libglad.a ${CMAKE_SOURCE_DIR}/vendor/stb/lib/libstb_image.a dl)
