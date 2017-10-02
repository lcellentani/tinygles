include(${PROJECT_SOURCE_DIR}/sources/CMakeCommon.cmake)

# The version number.
#set (TINYNGINE_VERSION_MAJOR 1)
#set (TINYNGINE_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
#configure_file (
#	"${CMAKE_CURRENT_SOURCE_DIR}/Config.h.in"
#	"${CMAKE_CURRENT_SOURCE_DIR}/Config.h"
#)

file(GLOB TINYNGINE_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h*")

file(GLOB TINYNGINE_IMGUI_INCLUDES "${PROJECT_SOURCE_DIR}/3rdparty/imgui/*.h*")
file(GLOB TINYNGINE_IMGUI_SOURCES "${PROJECT_SOURCE_DIR}/3rdparty/imgui/*.c*")

file(GLOB TINYNGINE_TINYOBJLOADER_INCLUDES "${PROJECT_SOURCE_DIR}/3rdparty/tinyobjloader/*.h*")
file(GLOB TINYNGINE_TINYOBJLOADER_SOURCES "${PROJECT_SOURCE_DIR}/3rdparty/tinyobjloader/*.c*")

file(GLOB TINYNGINE_PRIVATE_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h*")

if(MSVC) 
	list(APPEND TINYNGINE_ALL_INCLUDES ${TINYNGINE_INCLUDES}) 
	list(APPEND TINYNGINE_ALL_INCLUDES ${TINYNGINE_IMGUI_INCLUDES})
	list(APPEND TINYNGINE_ALL_INCLUDES ${TINYNGINE_TINYOBJLOADER_INCLUDES})
endif()

add_library(tinyngine
	${TINYNGINE_ALL_INCLUDES}
	${TINYNGINE_PRIVATE_INCLUDES}
	${PROJECT_SOURCE_DIR}/3rdparty/imgui/imgui.cpp
	${PROJECT_SOURCE_DIR}/3rdparty/imgui/imgui_demo.cpp
	${PROJECT_SOURCE_DIR}/3rdparty/imgui/imgui_draw.cpp
	${PROJECT_SOURCE_DIR}/3rdparty/tinyobjloader/tiny_obj_loader.cc
	src/main.cpp
	src/Application.cpp
	src/DynLibLoader.cpp
	src/EGLPlatformContext.cpp
	src/EGLTrampoline.cpp
	src/GeometryUtil.cpp
	src/GLApi.cpp
	src/IndexBufferGL.cpp
	src/Log.cpp
	src/PlatformBridgeWin32.cpp
	src/ProgramGL.cpp
	src/RendererGL.cpp
	src/ShaderGL.cpp
	src/ShadersUtil.cpp
	src/StopWatchWin32.cpp
	src/StringUtils.cpp
	src/TransformHelper.cpp
	src/VertexBufferGL.cpp
	src/VertexFormat.cpp
)

if(MSVC)
	source_group("Header Files\\" FILES ${TINYNGINE_INCLUDES}) 
	source_group("Source Files\\" FILES ${TINYNGINE_PRIVATE_INCLUDES})
	source_group("Header Files\\imgui" FILES ${TINYNGINE_IMGUI_INCLUDES})
	source_group("Source Files\\imgui" FILES ${TINYNGINE_IMGUI_SOURCES})
	source_group("Header Files\\tinyobjloader" FILES ${TINYNGINE_TINYOBJLOADER_INCLUDES})
	source_group("Source Files\\tinyobjloader" FILES ${TINYNGINE_TINYOBJLOADER_SOURCES})
endif()

Enable_Cpp11(tinyngine)
AddCompilerFlags(tinyngine)

target_include_directories(tinyngine
	PUBLIC
		"${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE
		"${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(tinyngine
	libEGL
	libGLESv2
)

