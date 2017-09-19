
add_executable(02-directionallighting
    DirectionalLighting.cpp
)

target_link_libraries(02-directionallighting
    tinyngine
)

set_target_properties(02-directionallighting
    PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/media"
)

Enable_Cpp11(02-directionallighting)
AddCompilerFlags(02-directionallighting)

SetLinkerSubsystem(02-directionallighting)