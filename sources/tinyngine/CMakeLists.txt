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
file(GLOB TINYNGINE_PRIVATE_GL_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/src/gl/*.h*")
file(GLOB TINYNGINE_PRIVATE_GL_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/gl/*.c*")

if(MSVC) 
	list(APPEND TINYNGINE_ALL_INCLUDES ${TINYNGINE_INCLUDES}) 
	list(APPEND TINYNGINE_ALL_INCLUDES ${TINYNGINE_IMGUI_INCLUDES})
	list(APPEND TINYNGINE_ALL_INCLUDES ${TINYNGINE_TINYOBJLOADER_INCLUDES})
endif()

add_library(tinyngine
	${TINYNGINE_ALL_INCLUDES}
	${TINYNGINE_PRIVATE_INCLUDES}
	${TINYNGINE_PRIVATE_GL_INCLUDES}
	${PROJECT_SOURCE_DIR}/3rdparty/imgui/imgui.cpp
	${PROJECT_SOURCE_DIR}/3rdparty/imgui/imgui_demo.cpp
	${PROJECT_SOURCE_DIR}/3rdparty/imgui/imgui_draw.cpp
	${PROJECT_SOURCE_DIR}/3rdparty/tinyobjloader/tiny_obj_loader.cc
	src/mainWin32.cpp
	src/Application.cpp
	src/DynLibLoader.cpp
	src/Engine.cpp
	src/ImageLoader.cpp
	src/ImGUIWrapper.cpp
	src/InputWin32.cpp
	src/Log.cpp
	src/MeshLoader.cpp
	src/StringUtils.cpp
	src/Time.cpp
	src/TransformHelper.cpp
	src/VertexFormat.cpp
	src/gl/EGLPlatformContext.cpp
	src/gl/EGLTrampoline.cpp
	src/gl/GLApi.cpp
	src/gl/GraphicsDeviceGL.cpp
	src/gl/IndexBufferGL.cpp
	src/gl/ProgramGL.cpp
	src/gl/TextureGL.cpp
	src/gl/ShaderGL.cpp
	src/gl/VertexBufferGL.cpp
)

if(MSVC)
	source_group("Header Files\\" FILES ${TINYNGINE_INCLUDES}) 
	source_group("Source Files\\" FILES ${TINYNGINE_PRIVATE_INCLUDES})
	source_group("Source Files\\gl" FILES ${TINYNGINE_PRIVATE_GL_INCLUDES})
	source_group("Source Files\\gl" FILES ${TINYNGINE_PRIVATE_GL_SOURCES})
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

