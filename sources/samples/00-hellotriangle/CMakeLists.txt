
add_executable(00-hellotriangle
    HelloTriangle.cpp
)

target_link_libraries(00-hellotriangle
    tinygles
)

set_target_properties(00-hellotriangle
    PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/media"
)

Enable_Cpp11()
AddCompilerFlags(00-hellotriangle)

SetLinkerSubsystem(00-hellotriangle)