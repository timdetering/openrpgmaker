TEMPLATE = lib
TARGET = importerplugin
DEPENDPATH += .
INCLUDEPATH += ../../
INCLUDEPATH += ../../../../fmodex/inc
LIBS += -L. -L../../../../fmodex/lib
DEFINES += VERSION=\\\"0.2.3\\\" TODO_LEVEL=0 FILE_FORMAT_VERSION=3
CONFIG += debug staticlib
DESTDIR = ../
*-64 {
	DEFINES += ARCH_TYPE=\\\"64\\\"
} else {
	DEFINES += ARCH_TYPE=\\\"32\\\"
}

# Input
HEADERS += ../../audiomanager.h \
           importerplugin.h \
           ../../menuframe.h \
           ../../menuimage.h \
           ../../menuitemtable.h \
           ../../menuobject.h \
           ../../menuselectablearea.h \
           ../../menuscrollarea.h \
           ../../menutext.h \
           ../../projectdata.h \
           ../../projectdata_attribute.h \
           ../../projectdata_baseevent.h \
           ../../projectdata_battleanimation.h \
           ../../projectdata_battleevent.h \
           ../../projectdata_battlelayout.h \
           ../../projectdata_character.h \
           ../../projectdata_charactersprite.h \
           ../../projectdata_class.h \
           ../../projectdata_commonevent.h \
           ../../projectdata_condition.h \
           ../../projectdata_font.h \
           ../../projectdata_globalanimation.h \
           ../../projectdata_introscreen.h \
           ../../projectdata_item.h \
           ../../projectdata_mainmenulayout.h \
           ../../projectdata_map.h \
           ../../projectdata_mapevent.h \
           ../../projectdata_menunavigation.h \
           ../../projectdata_monster.h \
           ../../projectdata_monsteranimation.h \
           ../../projectdata_monstergroup.h \
           ../../projectdata_skill.h \
           ../../projectdata_popupmenulayout.h \
           ../../projectdata_terrain.h \
           ../../projectdata_tile.h \
           ../../projectdata_tileset.h \
           ../../projectdata_titlemenulayout.h \
           ../../projectdata_vehicle.h \
					 ../../storagefile.h \
           ../../xmlTools.h \
           ../../xmlParser.h
SOURCES += ../../audiomanager.cpp \
           importerplugin.cpp \
           ../../menuframe.cpp \
           ../../menuimage.cpp \
           ../../menuitemtable.cpp \
           ../../menuobject.cpp \
           ../../menuselectablearea.cpp \
           ../../menuscrollarea.cpp \
           ../../menutext.cpp \
           ../../projectdata.cpp \
           ../../projectdata_attribute.cpp \
           ../../projectdata_baseevent.cpp \
           ../../projectdata_battleanimation.cpp \
           ../../projectdata_battleevent.cpp \
           ../../projectdata_battlelayout.cpp \
           ../../projectdata_character.cpp \
           ../../projectdata_charactersprite.cpp \
           ../../projectdata_class.cpp \
           ../../projectdata_commonevent.cpp \
           ../../projectdata_condition.cpp \
           ../../projectdata_font.cpp \
           ../../projectdata_globalanimation.cpp \
           ../../projectdata_introscreen.cpp \
           ../../projectdata_item.cpp \
           ../../projectdata_mainmenulayout.cpp \
           ../../projectdata_map.cpp \
           ../../projectdata_mapevent.cpp \
           ../../projectdata_menunavigation.cpp \
           ../../projectdata_monster.cpp \
           ../../projectdata_monsteranimation.cpp \
           ../../projectdata_monstergroup.cpp \
           ../../projectdata_popupmenulayout.cpp \
           ../../projectdata_skill.cpp \
           ../../projectdata_terrain.cpp \
           ../../projectdata_tile.cpp \
           ../../projectdata_tileset.cpp \
           ../../projectdata_titlemenulayout.cpp \
           ../../projectdata_vehicle.cpp \
					 ../../storagefile.cpp \
           ../../xmlTools.cpp \
           ../../xmlParser.cpp
