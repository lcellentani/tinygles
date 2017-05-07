# The version number.
set (TINYGLES_VERSION_MAJOR 1)
set (TINYGLES_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
	"${CMAKE_CURRENT_SOURCE_DIR}/Config.h.in"
	"${CMAKE_CURRENT_SOURCE_DIR}/Config.h"
)

set(CMAKE_CXX_FLAGS "/W3 /MP /Gy /GR- /arch:SSE2 /wd4201 /wd4324")

add_definitions(-DWIN32 -D_WIN32 -D_HAS_EXCEPTIONS=0 -D_SCL_SECURE=0 -D_SECURE_SCL=0 -D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE)
if (MSVC14)
add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
else()
add_definitions(-D_HAS_ITERATOR_DEBUGGING=0)
endif()

include_directories("${PROJECT_SOURCE_DIR}/3rdparty/khronos/include")
include_directories("${PROJECT_SOURCE_DIR}/sources")

file(GLOB INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(tinygles ${SOURCES} ${INCLUDES})
#target_link_libraries(tinygles libEGL libGLESv2)
