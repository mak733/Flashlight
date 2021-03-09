add_library(IPlugins INTERFACE
    core_interface.h
    defines.h
    header.h)

target_include_directories(IPlugins INTERFACE .)
target_link_libraries(IPlugins INTERFACE ${REQUIRED_LIBS_Qt_QUALIFIED})
