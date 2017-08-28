include(${PROJECT_SOURCE_DIR}/sources/CMakeCommon.cmake)

# The version number.
#set (TINYGLES_VERSION_MAJOR 1)
#set (TINYGLES_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
#configure_file (
#	"${CMAKE_CURRENT_SOURCE_DIR}/Config.h.in"
#	"${CMAKE_CURRENT_SOURCE_DIR}/Config.h"
#)

file(GLOB TINYGLES_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h*")

file(GLOB TINYGLES_IMGUI_INCLUDES "${PROJECT_SOURCE_DIR}/3rdparty/imgui/*.h*")
file(GLOB TINYGLES_IMGUI_SOURCES "${PROJECT_SOURCE_DIR}/3rdparty/imgui/*.c*")

file(GLOB TINYGLES_PRIVATE_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h*")

if(MSVC) 
	list(APPEND TINYGLES_ALL_INCLUDES ${TINYGLES_INCLUDES}) 
	list(APPEND TINYGLES_ALL_INCLUDES ${TINYGLES_IMGUI_INCLUDES}) 
endif()

add_library(tinygles
	${TINYGLES_ALL_INCLUDES}
	${TINYGLES_PRIVATE_INCLUDES}
	${PROJECT_SOURCE_DIR}/3rdparty/imgui/imgui.cpp
	${PROJECT_SOURCE_DIR}/3rdparty/imgui/imgui_demo.cpp
	${PROJECT_SOURCE_DIR}/3rdparty/imgui/imgui_draw.cpp
	src/main.cpp
	src/Application.cpp
	src/DynLibLoader.cpp
	src/EGLTrampoline.cpp
	src/Log.cpp
	src/EGLPlatformContext.cpp
	src/PlatformBridge.cpp
	src/Shapes.cpp
	src/StringUtils.cpp
)

if(MSVC)
	source_group("Header Files\\" FILES ${TINYGLES_INCLUDES}) 
	source_group("Source Files\\" FILES ${TINYGLES_PRIVATE_INCLUDES})
	source_group("Header Files\\imgui" FILES ${TINYGLES_IMGUI_INCLUDES})
	source_group("Source Files\\imgui" FILES ${TINYGLES_IMGUI_SOURCES})
endif()

Enable_Cpp11()
AddCompilerFlags(tinygles)

target_include_directories(tinygles
	PUBLIC
		"${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE
		"${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(tinygles
	libEGL
	libGLESv2
)

