
add_executable(03-pixellighting
    PerPixelLighting.cpp
)

target_link_libraries(03-pixellighting
    tinyngine
)

set_target_properties(03-pixellighting
    PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/media"
)

Enable_Cpp11(03-pixellighting)
AddCompilerFlags(03-pixellighting)

SetLinkerSubsystem(03-pixellighting)