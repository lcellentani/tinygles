cmake_minimum_required (VERSION 2.6)
project (tinygles)

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

set(CMAKE_CXX_FLAGS "/W3 /MP /Gy /GR- /arch:SSE2 /wd4201 /wd4324")

add_definitions(-D_WIN32 -D_HAS_EXCEPTIONS=0 -D_SCL_SECURE=0 -D_SECURE_SCL=0 -D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE)
if (MSVC14)
add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
else()
add_definitions(-D_HAS_ITERATOR_DEBUGGING=0)
endif()

add_subdirectory("${PROJECT_SOURCE_DIR}/sources")
add_subdirectory("${PROJECT_SOURCE_DIR}/samples/00-hellotriangle")
add_subdirectory("${PROJECT_SOURCE_DIR}/samples/01-vertexshader")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT 00-hellotriangle)