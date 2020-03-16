CONFIG += c++11
TEMPLATE = lib
CONFIG += plugin

INCLUDEPATH += \
    ../

DESTDIR = ../libs/

MOC_DIR = .moc/

OBJECTS_DIR = .obj/

QMAKE_CLEAN += $${DESTDIR}lib$${TARGET}.so
