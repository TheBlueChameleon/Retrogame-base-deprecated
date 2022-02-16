cmake_minimum_required(VERSION 3.5)

# ============================================================================ #
# lanugage definition

project(Retrogame-base LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================================ #
# in-code macro definition

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_definitions(-DDEBUG)
endif(${CMAKE_BUILD_TYPE} MATCHES "Debug")

add_definitions(-DPROJECT_NAME="Retrogame-base")
add_definitions(-DCODEVERSION_MAJOR=1)
add_definitions(-DCODEVERSION_MINOR=1)

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
    #
    src/gfxSystem/window.hpp src/gfxSystem/window.cpp
    src/gfxSystem/texturestore.hpp src/gfxSystem/texturestore.cpp
    src/gfxSystem/animation.hpp src/gfxSystem/animation.cpp
    src/gfxSystem/animationstore.hpp src/gfxSystem/animationstore.cpp
    src/gfxSystem/animationlayer.hpp src/gfxSystem/animationlayer.cpp
    #
    src/xmlSystem/pugixml.hpp src/xmlSystem/pugiconfig.hpp src/xmlSystem/pugixml.cpp
    src/xmlSystem/xmlwrapper.hpp src/xmlSystem/xmlwrapper.cpp
)

target_link_libraries(Retrogame-base ${SDL2_LIBRARIES})

# ============================================================================ #
# unittest definition

add_executable(Retrogame-base-unittest
    src/main-unittest.cpp
    src/unittests/unittest-macros.hpp
    src/globals.hpp src/globals.cpp
    #
    src/gfxSystem/window.hpp src/gfxSystem/window.cpp
    src/gfxSystem/texturestore.hpp src/gfxSystem/texturestore.cpp
    src/gfxSystem/animation.hpp src/gfxSystem/animation.cpp
    src/gfxSystem/animationstore.hpp src/gfxSystem/animationstore.cpp
    #
    src/xmlSystem/pugixml.hpp src/xmlSystem/pugiconfig.hpp src/xmlSystem/pugixml.cpp
    src/xmlSystem/xmlwrapper.hpp src/xmlSystem/xmlwrapper.cpp
    #
    src/unittests/globals-unittest.hpp src/unittests/globals-unittest.cpp
    src/unittests/xmlsystem-unittest.hpp src/unittests/xmlsystem-unittest.cpp
    src/unittests/window-unittest.hpp src/unittests/window-unittest.cpp
    src/unittests/texturestore-unittest.hpp src/unittests/texturestore-unittest.cpp
    src/unittests/animation-unittest.hpp src/unittests/animation-unittest.cpp
    src/unittests/animationstore-unittest.hpp src/unittests/animationstore-unittest.cpp
)

target_link_libraries(Retrogame-base-unittest ${SDL2_LIBRARIES})
