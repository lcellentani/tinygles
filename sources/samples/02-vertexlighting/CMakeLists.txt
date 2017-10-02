
add_executable(02-vertexlighting
    PerVertexLighting.cpp
)

target_link_libraries(02-vertexlighting
    tinyngine
)

set_target_properties(02-vertexlighting
    PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/media"
)

Enable_Cpp11(02-vertexlighting)
AddCompilerFlags(02-vertexlighting)

SetLinkerSubsystem(02-vertexlighting)