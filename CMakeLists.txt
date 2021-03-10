cmake_minimum_required(VERSION 3.10)
project(TestCmakeQtProject)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QT_VERSION 5)
set(REQUIRED_LIBS_Qt Core Gui Widgets Network Xml Svg)
set(REQUIRED_LIBS_Qt_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network Qt5::Xml Qt5::Svg)
find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS_Qt} REQUIRED)

qt5_add_resources(RESOURCE_FILES russia.qrc)
set(SOURCES connectiondialog.cpp
    flashlightwidget.cpp
    main.cpp
    mainwindow.cpp
    plugins/pluginmanager.cpp
    ${RESOURCE_FILES})
add_executable(${PROJECT_NAME} ${SOURCES})

if (NOT CMAKE_PREFIX_PATH)
    message(WARNING "CMAKE_PREFIX_PATH is not defined, you may need to set it "
            "(-DCMAKE_PREFIX_PATH=\"path/to/Qt/lib/cmake\" or -DCMAKE_PREFIX_PATH=/usr/include/{host}/qt{version}/ on Ubuntu)")
endif ()


add_subdirectory(plugins/Type-length-value/Interface)
add_subdirectory(plugins/Type-length-value/OnPlugin)
add_subdirectory(plugins/Type-length-value/OffPlugin)
add_subdirectory(plugins/Type-length-value/ColorPlugin)
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_Qt_QUALIFIED} IPlugins)

