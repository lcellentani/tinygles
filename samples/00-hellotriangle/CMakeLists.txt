set(CMAKE_CXX_FLAGS "/W3 /MP /Gy /GR- /arch:SSE2 /wd4201 /wd4324")

add_definitions(-D_WIN32 -D_HAS_EXCEPTIONS=0 -D_SCL_SECURE=0 -D_SECURE_SCL=0 -D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE)
if (MSVC14)
add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
else()
add_definitions(-D_HAS_ITERATOR_DEBUGGING=0)
endif()

include_directories("${PROJECT_SOURCE_DIR}/3rdparty/khronos/include")
include_directories("${PROJECT_SOURCE_DIR}/sources")

file(GLOB INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

link_directories("${PROJECT_SOURCE_DIR}/3rdparty/khronos/lib")

add_executable(00-hellotriangle ${SOURCES} ${INCLUDES})
target_link_libraries(00-hellotriangle tinygles libEGL libGLESv2)
set_target_properties(00-hellotriangle PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/media")

if(WIN32) # Check if we are on Windows
  if(MSVC) # Check if we are using the Visual Studio compiler
    set_target_properties(00-hellotriangle PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:\"mainCRTStartup\"")
  elseif(CMAKE_COMPILER_IS_GNUCXX)
    # SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows") # Not tested
  else()
    message(SEND_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
  endif()
elseif(UNIX)
  # Nothing special required
else()
  message(SEND_ERROR "You are on an unsupported platform! (Not Win32 or Unix)")
endif()