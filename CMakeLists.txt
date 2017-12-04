cmake_minimum_required (VERSION 3.2 FATAL_ERROR)
project (tinyngine VERSION 0.1 LANGUAGES CXX)

set(CMAKE_SUPPRESS_REGENERATION true)
# explicitely specify which configurations we want
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

# make all binary artifacts end up on the same place
set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}/3rdparty/khronos")
include_directories("${PROJECT_SOURCE_DIR}/3rdparty/glm")
include_directories("${PROJECT_SOURCE_DIR}/3rdparty/imgui")
include_directories("${PROJECT_SOURCE_DIR}/3rdparty/stb")
include_directories("${PROJECT_SOURCE_DIR}/3rdparty/tinyobjloader")

link_directories("${PROJECT_SOURCE_DIR}/3rdparty/gles/angle/lib")

add_subdirectory(sources/tinyngine)
add_subdirectory(sources/samples/common)
add_subdirectory(sources/samples/00-hellotriangle)
add_subdirectory(sources/samples/01-vertexshader)
add_subdirectory(sources/samples/02-gouraudshading)
add_subdirectory(sources/samples/03-phongshading)
add_subdirectory(sources/samples/04-multitexture)
#add_subdirectory(sources/samples/05-simplemipmapping)

if (MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT 00-hellotriangle)
endif()