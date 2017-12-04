
add_executable(02-gouraudshading
    GouraudShading.cpp
)

target_link_libraries(02-gouraudshading
    tinyngine
)

set_target_properties(02-gouraudshading
    PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/media"
)

SetupSample(02-gouraudshading)

Enable_Cpp11(02-gouraudshading)
AddCompilerFlags(02-gouraudshading)

SetLinkerSubsystem(02-gouraudshading)