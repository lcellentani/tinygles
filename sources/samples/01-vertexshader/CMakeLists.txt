
add_executable(01-vertexshader
    VertexShader.cpp
)

target_link_libraries(01-vertexshader
    tinygles
)

set_target_properties(01-vertexshader
    PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/media"
)

Enable_Cpp11()
AddCompilerFlags(01-vertexshader)

SetLinkerSubsystem(01-vertexshader)