
add_executable(04-multitexture
    MultiTexture.cpp
)

target_link_libraries(04-multitexture
    tinyngine
)

set_target_properties(04-multitexture
    PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/media"
)

Enable_Cpp11(04-multitexture)
AddCompilerFlags(04-multitexture)

SetLinkerSubsystem(04-multitexture)