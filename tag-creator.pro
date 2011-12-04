TEMPLATE	 = app
CONFIG		+= meegotouch mobility debug
#CONFIG		+= console
MOBILITY	+= connectivity contacts systeminfo versit
QT		+= core
QT		+= dbus

TARGET		 = tag-creator

PREFIX		 = /opt/tag-creator

QMAKE_CXXFLAGS	+= -Werror -DINSTALLPREFIX='\\\"/opt/tag-creator\\\"'

OBJECTS_DIR	= ./obj
MOC_DIR		= ./moc
DESTDIR		= ./out
	   
HEADERS		+= \
		src/BluezDevice.h \
		src/BluezSupplicant.h \
		src/BtNdefRecord.h \
		src/BtPage.h \
		src/BtSelectionPage.h \
		src/BtSelectionPageListCellCreator.h \
		src/BtSelectionPageExistingListModel.h \
		src/BtSelectionPageScanListModel.h \
		src/BtSelectionPageListModel.h \
		src/ContactPage.h \
		src/ContactSelectionPage.h \
		src/ContactSelectionPageListCellCreator.h \
		src/ContactSelectionPageListModel.h \
		src/CreatePage.h \
		src/LabeledTextEdit.h \
		src/MainPage.h \
		src/SelectionPage.h \
		src/Tag.h \
		src/TagListCellCreator.h \
		src/TagListModel.h \
		src/TagTypeListCellCreator.h \
		src/TagTypeListModel.h \
		src/TagStorage.h \
		src/TextPage.h \
		src/TextRecordEdit.h \
		src/UrlPage.h \
		src/VCardNdefRecord.h

SOURCES		+= \
		src/tag-creator.cpp \
		src/BluezDevice.cpp \
		src/BluezSupplicant.cpp \
		src/BtNdefRecord.cpp \
		src/BtPage.cpp \
		src/BtSelectionPage.cpp \
		src/BtSelectionPageListCellCreator.cpp \
		src/BtSelectionPageExistingListModel.cpp \
		src/BtSelectionPageScanListModel.cpp \
		src/BtSelectionPageListModel.cpp \
		src/ContactPage.cpp \
		src/ContactSelectionPage.cpp \
		src/ContactSelectionPageListCellCreator.cpp \
		src/ContactSelectionPageListModel.cpp \
		src/CreatePage.cpp \
		src/LabeledTextEdit.cpp \
		src/MainPage.cpp \
		src/SelectionPage.cpp \
		src/Tag.cpp \
		src/TagListCellCreator.cpp \
		src/TagListModel.cpp \
		src/TagTypeListCellCreator.cpp \
		src/TagTypeListModel.cpp \
		src/TagStorage.cpp \
		src/TextPage.cpp \
		src/TextRecordEdit.cpp \
		src/UrlPage.cpp \
		src/VCardNdefRecord.cpp

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
