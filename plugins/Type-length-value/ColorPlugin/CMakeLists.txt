project (ColorPlugin)
set(QT_VERSION 5)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../libs/tlv/)
add_library( ${PROJECT_NAME} SHARED colorplugin.cpp
    colorplugin.h
    codogramm.h)
target_include_directories( ${PROJECT_NAME} PUBLIC .)

target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_Qt_QUALIFIED} IPlugins)

