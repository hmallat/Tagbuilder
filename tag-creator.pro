TEMPLATE         = app
CONFIG          += meegotouch mobility debug
#CONFIG         += console
MOBILITY        += connectivity systeminfo
QT              += core

TARGET           = tag-creator

PREFIX           = /opt/tag-creator

QMAKE_CXXFLAGS  += -Werror -DINSTALLPREFIX='\\\"/opt/tag-creator\\\"'

DEPENDPATH      += ./src
INCLUDEPATH     += ./src
VPATH           += $$DEPENDPATH $$INCLUDEPATH

OBJECTS_DIR     = ./obj
MOC_DIR         = ./moc
DESTDIR         = ./out
           
HEADERS         += \
                CreatePage.h \
                CreateTextPage.h \
                MainPage.h \
		Tag.h \
		TagStorage.h

SOURCES         += \
                tag-creator.cpp \
                CreatePage.cpp \
                CreateTextPage.cpp \
                MainPage.cpp \
		Tag.cpp \
		TagStorage.cpp

TRANSLATIONS    = \
                tag-creator_en.ts \
                tag-creator_fi.ts

target.path     = $$PREFIX/bin

gfx.path        = $$PREFIX/share/images
gfx.files       = \

trans.path      = $$PREFIX/share/l10n/tag-writing-plugin
trans.files     = \
                tag-creator_en.qm \
                tag-creator_fi.qm

INSTALLS        += target gfx trans
