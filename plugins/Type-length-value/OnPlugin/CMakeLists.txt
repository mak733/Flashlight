project (OnPlugin)
set(QT_VERSION 5)

add_library( ${PROJECT_NAME} SHARED onplugin.cpp
    onplugin.h
    codogramm.h)
target_include_directories( ${PROJECT_NAME} PUBLIC .)

target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_Qt_QUALIFIED} IPlugins)
