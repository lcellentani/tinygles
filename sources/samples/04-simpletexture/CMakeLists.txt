
add_executable(03-simpletexture
    SimpleTexture.cpp
)

target_link_libraries(03-simpletexture
    tinyngine
)

set_target_properties(03-simpletexture
    PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/media"
)

Enable_Cpp11(03-simpletexture)
AddCompilerFlags(03-simpletexture)

SetLinkerSubsystem(03-simpletexture)