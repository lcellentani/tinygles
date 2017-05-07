# The version number.
set (TINYGLES_VERSION_MAJOR 1)
set (TINYGLES_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
	"${CMAKE_CURRENT_SOURCE_DIR}/Config.h.in"
	"${CMAKE_CURRENT_SOURCE_DIR}/Config.h"
)

include_directories("${PROJECT_SOURCE_DIR}/sources")

file(GLOB INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(tinygles ${SOURCES} ${INCLUDES})
target_link_libraries(tinygles libEGL libGLESv2)
