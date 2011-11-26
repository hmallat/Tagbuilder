TEMPLATE	 = app
CONFIG		+= meegotouch mobility debug
#CONFIG		+= console
MOBILITY	+= connectivity systeminfo
QT		+= core

TARGET		 = tag-creator

PREFIX		 = /opt/tag-creator

QMAKE_CXXFLAGS	+= -Werror -DINSTALLPREFIX='\\\"/opt/tag-creator\\\"'

OBJECTS_DIR	= ./obj
MOC_DIR		= ./moc
DESTDIR		= ./out
	   
HEADERS		+= \
		src/CreatePage.h \
		src/LabeledTextEdit.h \
		src/MainPage.h \
		src/Tag.h \
		src/TagListCellCreator.h \
		src/TagListModel.h \
		src/TagTypeListCellCreator.h \
		src/TagTypeListModel.h \
		src/TagStorage.h \
		src/TextPage.h \
		src/TextRecordEdit.h

SOURCES		+= \
		src/tag-creator.cpp \
		src/CreatePage.cpp \
		src/LabeledTextEdit.cpp \
		src/MainPage.cpp \
		src/Tag.cpp \
		src/TagListCellCreator.cpp \
		src/TagListModel.cpp \
		src/TagTypeListCellCreator.cpp \
		src/TagTypeListModel.cpp \
		src/TagStorage.cpp \
		src/TextPage.cpp \
		src/TextRecordEdit.cpp

TRANSLATIONS	= \
		tag-creator_en.ts \
		tag-creator_fi.ts

target.path	= $$PREFIX/bin

gfx.path	= $$PREFIX/share/images
gfx.files	= \

trans.path	= $$PREFIX/share/l10n/tag-writing-plugin
trans.files	= \
		tag-creator_en.qm \
		tag-creator_fi.qm

INSTALLS	+= target gfx trans
