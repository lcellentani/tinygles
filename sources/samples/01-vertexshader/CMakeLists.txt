include_directories("${PROJECT_SOURCE_DIR}/sources")

file(GLOB INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(01-vertexshader ${SOURCES} ${INCLUDES})
target_link_libraries(01-vertexshader tinygles)
set_target_properties(01-vertexshader PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/media")

if(WIN32) # Check if we are on Windows
  if(MSVC) # Check if we are using the Visual Studio compiler
    set_target_properties(01-vertexshader PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:\"mainCRTStartup\"")
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