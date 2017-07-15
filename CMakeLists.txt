cmake_minimum_required (VERSION 3.2 FATAL_ERROR)
project (tinygles VERSION 0.1 LANGUAGES CXX)

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

include_directories("${PROJECT_SOURCE_DIR}/3rdparty/khronos/include")
link_directories("${PROJECT_SOURCE_DIR}/3rdparty/khronos/lib")

add_subdirectory(sources/tinygles)
add_subdirectory(sources/samples/00-hellotriangle)
add_subdirectory(sources/samples/01-vertexshader)

if (MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT 00-hellotriangle)
endif()