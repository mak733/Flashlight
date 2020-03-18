CONFIG += c++11 plugin
TEMPLATE = lib

INCLUDEPATH += \
    ../../

DESTDIR = ../../libs/tlv/

MOC_DIR = $$(PWD)/.moc/

OBJECTS_DIR = $$(PWD)/.obj/

QMAKE_CLEAN += $${DESTDIR}lib$${TARGET}.so
