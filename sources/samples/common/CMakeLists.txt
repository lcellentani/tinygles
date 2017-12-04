include(${PROJECT_SOURCE_DIR}/sources/CMakeCommon.cmake)

file(GLOB EXAMPLES_COMMON_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/*.h*")

if(MSVC) 
	list(APPEND EXAMPLES_COMMON_ALL_INCLUDES ${EXAMPLES_COMMON_INCLUDES}) 
endif()

add_library(examples-common
	${EXAMPLES_COMMON_ALL_INCLUDES}
	ExampleBaseApp.cpp
)

if(MSVC)
	source_group("Header Files\\" FILES ${EXAMPLES_COMMON_INCLUDES}) 
endif()

target_include_directories(examples-common
	PUBLIC
		"${PROJECT_SOURCE_DIR}/sources/tinyngine/include"
)

Enable_Cpp11(examples-common)
AddCompilerFlags(examples-common)

