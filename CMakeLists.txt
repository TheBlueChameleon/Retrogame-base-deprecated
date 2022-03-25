cmake_minimum_required(VERSION 3.5)

# ============================================================================ #
# lanugage definition

project(Retrogamebase
    VERSION 0.0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================================ #
# in-code macro definition

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_definitions(-DDEBUG)
endif(${CMAKE_BUILD_TYPE} MATCHES "Debug")

add_definitions(-DPROJECT_NAME="Retrogame")
add_definitions(-DCODEVERSION_MAJOR=1)
add_definitions(-DCODEVERSION_MINOR=1)

# ============================================================================ #
# load SDL2

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake-sdl2)

if(NOT DEFINED CMAKE_SUPPRESS_DEVELOPER_WARNINGS)
     set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings")
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

#set(SDL2_LIBRARIES "${SDL2_LIBRARIES} -lSDL2_image -lSDL2_ttf")

include_directories(${SDL2_INCLUDE_DIRS})

# ============================================================================ #
# library definition

add_library(Retrogamebase-lib SHARED
    src/Retrogamebase.h
    #
    src/base/globals.hpp src/base/globals.cpp
    src/base/exceptions.hpp
    #
    src/gfxSystem/window.hpp src/gfxSystem/window.cpp
    src/gfxSystem/texturestore.hpp src/gfxSystem/texturestore.cpp
    src/gfxSystem/animation.hpp src/gfxSystem/animation.cpp
    src/gfxSystem/animationstore.hpp src/gfxSystem/animationstore.cpp
    src/gfxSystem/animationlayer.hpp src/gfxSystem/animationlayer.cpp
    src/gfxSystem/animationlayerstore.hpp src/gfxSystem/animationlayerstore.cpp
    #
    src/gfxSystem/effects/visualeffect.hpp src/gfxSystem/effects/visualeffect.cpp
    src/gfxSystem/effects/simplefadeout.hpp src/gfxSystem/effects/simplefadeout.cpp
    src/gfxSystem/effects/stripesfadeout.hpp src/gfxSystem/effects/stripesfadeout.cpp
    #
    src/xmlSystem/pugixml.hpp src/xmlSystem/pugiconfig.hpp src/xmlSystem/pugixml.cpp
    src/xmlSystem/xmlwrapper.hpp src/xmlSystem/xmlwrapper.cpp
)

set_target_properties(Retrogamebase-lib PROPERTIES
    VERSION   ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION}
    PUBLIC_HEADER src/Retrogamebase.h
)

target_link_libraries(Retrogamebase-lib
    SDL2::Main
    SDL2::Image
    SDL2::TTF
)

# ============================================================================ #
# main executable definition

add_executable(Retrogamebase-exe
    src/main.cpp
)

target_link_libraries(Retrogamebase-exe
    ${SDL2_LIBRARIES}
    Retrogamebase-lib
)

# ============================================================================ #
# unittest definition

add_executable(Retrogamebase-unittest
    src/main-unittest.cpp
    src/unittest-driver/unittest.hpp src/unittest-driver/unittest.cpp
    #
    src/unittests/unittest-globals.hpp src/unittests/unittest-globals.cpp
    src/unittests/unittest-xmlsystem.hpp src/unittests/unittest-xmlsystem.cpp
    src/unittests/unittest-window.hpp src/unittests/unittest-window.cpp
    src/unittests/unittest-texturestore.hpp src/unittests/unittest-texturestore.cpp
    src/unittests/unittest-animation.hpp src/unittests/unittest-animation.cpp
    src/unittests/unittest-animationstore.hpp src/unittests/unittest-animationstore.cpp
    src/unittests/unittest-animationlayer.hpp src/unittests/unittest-animationlayer.cpp
    src/unittests/unittest-animationlayerstore.hpp src/unittests/unittest-animationlayerstore.cpp
)

target_link_libraries(Retrogamebase-unittest
    ${SDL2_LIBRARIES}
    Retrogamebase-lib
)
