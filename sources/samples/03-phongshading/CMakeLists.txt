
add_executable(03-phongshading
    PhongShading.cpp
)

target_link_libraries(03-phongshading
    tinyngine
)

set_target_properties(03-phongshading
    PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/media"
)

Enable_Cpp11(03-phongshading)
AddCompilerFlags(03-phongshading)

SetLinkerSubsystem(03-phongshading)