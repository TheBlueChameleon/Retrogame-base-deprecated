cmake_minimum_required(VERSION 3.5)

# ============================================================================ #
# lanugage definition

project(Retrogame-base LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================================ #
# load SDL2

find_package(SDL2 REQUIRED)

set(SDL2_LIBRARIES "${SDL2_LIBRARIES} -lSDL2_image -lSDL2_ttf")

include_directories(${SDL2_INCLUDE_DIRS})

# ============================================================================ #
# main executable definition

add_executable(Retrogame-base
    src/main.cpp
    src/globals.hpp src/globals.cpp
    src/gfxSystem/window.hpp src/gfxSystem/window.cpp
    )

target_link_libraries(Retrogame-base ${SDL2_LIBRARIES})

# ============================================================================ #
# unittest definition

add_executable(Retrogame-base-unittest
    src/main-unittest.cpp
    src/globals.hpp src/globals.cpp
    src/gfxSystem/window.hpp src/gfxSystem/window.cpp
    src/unittests/window-unittest.hpp src/unittests/window-unittest.cpp
    src/unittests/gfxsystem-base-unittest.hpp src/unittests/gfxsystem-base-unittest.cpp
    )

target_link_libraries(Retrogame-base-unittest ${SDL2_LIBRARIES})
