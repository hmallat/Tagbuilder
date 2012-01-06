TEMPLATE	 = app
CONFIG		+= meegotouch mobility qtsparql debug
#CONFIG		+= console
MOBILITY	+= connectivity contacts organizer systeminfo versit
QT		+= core
QT		+= dbus

TARGET		 = tag-creator

PREFIX		 = /opt/tag-creator

QMAKE_CXXFLAGS	+= -Werror -DINSTALLPREFIX='\\\"/opt/tag-creator\\\"'
QMAKE_CXXFLAGS  += -DVERSION_MAJOR=0 -DVERSION_MINOR=0 -DVERSION_MICRO=3

OBJECTS_DIR	= ./obj
MOC_DIR		= ./moc
DESTDIR		= ./out
	   
HEADERS		+= \
		src/BluezDevice.h \
		src/BluezSupplicant.h \
		src/BookmarkSelectionPage.h \
		src/BookmarkSelectionPageListCellCreator.h \
		src/BookmarkSelectionPageListModel.h \
		src/BtNdefRecord.h \
		src/BtPage.h \
		src/BtSelectionPage.h \
		src/BtSelectionPageListCellCreator.h \
		src/BtSelectionPageExistingListModel.h \
		src/BtSelectionPageScanListModel.h \
		src/BtSelectionPageListModel.h \
		src/CalendarDetailCell.h \
		src/CalendarDetailPicker.h \
		src/CalendarDetailPickerListCellCreator.h \
		src/CalendarDetailPickerListModel.h \
		src/CalendarPage.h \
		src/CalendarSelectionPage.h \
		src/CalendarSelectionPageListCellCreator.h \
		src/CalendarSelectionPageListModel.h \
		src/ContactDetailCell.h \
		src/ContactDetailPicker.h \
		src/ContactDetailPickerListCellCreator.h \
		src/ContactDetailPickerListModel.h \
		src/ContactPage.h \
		src/ContactSelectionPage.h \
		src/ContactSelectionPageListCellCreator.h \
		src/ContactSelectionPageListModel.h \
		src/ContentItemCellCreator.h \
		src/CreatePage.h \
		src/CreateEditPage.h \
		src/LabeledTextEdit.h \
		src/LabelOrList.h \
		src/MainPage.h \
		src/NfcHandoverSelectNdefRecord.h \
		src/SelectionPage.h \
		src/SmartPosterRecord.h \
		src/Tag.h \
		src/TagListCellCreator.h \
		src/TagReader.h \
		src/TagTypeListCellCreator.h \
		src/TagTypeListModel.h \
		src/TagStorage.h \
		src/TagWriter.h \
		src/TextPage.h \
		src/TextRecordEdit.h \
		src/UnknownPage.h \
		src/UrlPage.h \
		src/Util.h \
		src/VCardNdefRecord.h \
		src/ViewHeader.h \
		src/WritePage.h

SOURCES		+= \
		src/tag-creator.cpp \
		src/BluezDevice.cpp \
		src/BluezSupplicant.cpp \
		src/BookmarkSelectionPage.cpp \
		src/BookmarkSelectionPageListCellCreator.cpp \
		src/BookmarkSelectionPageListModel.cpp \
		src/BtNdefRecord.cpp \
		src/BtPage.cpp \
		src/BtSelectionPage.cpp \
		src/BtSelectionPageListCellCreator.cpp \
		src/BtSelectionPageExistingListModel.cpp \
		src/BtSelectionPageScanListModel.cpp \
		src/BtSelectionPageListModel.cpp \
		src/CalendarDetailCell.cpp \
		src/CalendarDetailPicker.cpp \
		src/CalendarDetailPickerListCellCreator.cpp \
		src/CalendarDetailPickerListModel.cpp \
		src/CalendarPage.cpp \
		src/CalendarSelectionPage.cpp \
		src/CalendarSelectionPageListCellCreator.cpp \
		src/CalendarSelectionPageListModel.cpp \
		src/ContactDetailCell.cpp \
		src/ContactDetailPicker.cpp \
		src/ContactDetailPickerListCellCreator.cpp \
		src/ContactDetailPickerListModel.cpp \
		src/ContactPage.cpp \
		src/ContactSelectionPage.cpp \
		src/ContactSelectionPageListCellCreator.cpp \
		src/ContactSelectionPageListModel.cpp \
		src/ContentItemCellCreator.cpp \
		src/CreatePage.cpp \
		src/CreateEditPage.cpp \
		src/LabeledTextEdit.cpp \
		src/LabelOrList.cpp \
		src/MainPage.cpp \
		src/NfcHandoverSelectNdefRecord.cpp \
		src/SelectionPage.cpp \
		src/SmartPosterRecord.cpp \
		src/Tag.cpp \
		src/TagListCellCreator.cpp \
		src/TagReader.cpp \
		src/TagTypeListCellCreator.cpp \
		src/TagTypeListModel.cpp \
		src/TagStorage.cpp \
		src/TagWriter.cpp \
		src/TextPage.cpp \
		src/TextRecordEdit.cpp \
		src/UnknownPage.cpp \
		src/UrlPage.cpp \
		src/Util.cpp \
		src/VCardNdefRecord.cpp \
		src/ViewHeader.cpp \
		src/WritePage.cpp

TRANSLATIONS	= \
		tag-creator_en.ts \
		tag-creator_fi.ts

target.path	= $$PREFIX/bin

gfx.path	= $$PREFIX/share/images
gfx.files	= \
		data/tag-creator.jpeg \
		data/tap.png

trans.path	= $$PREFIX/share/l10n/tag-creator
trans.files	= \
		tag-creator_en.qm \
		tag-creator_fi.qm

desktop.path	= /usr/share/applications
desktop.files	= \
		data/tag-creator.desktop

style.path	= /usr/share/themes/base/meegotouch/tag-creator/style
style.files	= \
		data/tag-creator.css

INSTALLS	+= target gfx trans desktop style
