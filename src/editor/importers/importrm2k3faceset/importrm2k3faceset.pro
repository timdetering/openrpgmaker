TEMPLATE = lib
TARGET = $$qtLibraryTarget(importrm2k3faceset)
DEPENDPATH += .
INCLUDEPATH += ../importerplugin \
               ../../
INCLUDEPATH += ../../../../fmodex/inc
LIBS += -L. -L../../../../fmodex/lib
CONFIG += plugin debug
*-64 {
	DESTDIR = ../../../../data/importers64
	DEFINES += ARCH_TYPE=\\\"64\\\"
} else {
	DESTDIR = ../../../../data/importers32
	DEFINES += ARCH_TYPE=\\\"32\\\"
}
LIBS =  ../libimporterplugin.a
PRE_TARGETDEPS +=  ../libimporterplugin.a

# Input
HEADERS += importrm2k3faceset.h
SOURCES += importrm2k3faceset.cpp
