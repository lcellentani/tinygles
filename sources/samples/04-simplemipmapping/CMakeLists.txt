
add_executable(04-simplemipmapping
    SimpleMipmapping.cpp
)

target_link_libraries(04-simplemipmapping
    tinyngine
)

set_target_properties(04-simplemipmapping
    PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/media"
)

Enable_Cpp11(04-simplemipmapping)
AddCompilerFlags(04-simplemipmapping)

SetLinkerSubsystem(04-simplemipmapping)