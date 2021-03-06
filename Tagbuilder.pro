TEMPLATE	 = app
CONFIG		+= meegotouch mobility qtsparql debug
#CONFIG		+= console
MOBILITY	+= connectivity contacts location organizer systeminfo versit
QT		+= core dbus webkit

TARGET		 = Tagbuilder

PREFIX		 = /opt/Tagbuilder

QMAKE_CXXFLAGS	+= -Werror -DINSTALLPREFIX='\\\"/opt/Tagbuilder\\\"'

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
		src/CalendarDetailPicker.h \
		src/CalendarDetailPickerListCellCreator.h \
		src/CalendarDetailPickerListModel.h \
		src/CalendarPage.h \
		src/CalendarSelectionPage.h \
		src/CalendarSelectionPageListCellCreator.h \
		src/CalendarSelectionPageListModel.h \
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
		src/FoursquareAuthNetworkAccessManager.h \
		src/FoursquareAuthPage.h \
		src/FoursquareAuthStorage.h \
		src/FoursquareVenue.h \
		src/FoursquareVenueSearch.h \
		src/FoursquareVenueSelectionPage.h \
		src/FoursquareVenueSelectionPageListCellCreator.h \
		src/FoursquareVenueSelectionPageListModel.h \
		src/HelpPage.h \
		src/LabeledTextEdit.h \
		src/LabelOrList.h \
		src/MainPage.h \
		src/NfcdMonitor.h \
		src/NfcHandoverSelectNdefRecord.h \
		src/OrganizerItemHandler.h \
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
		src/UnknownRecordListCellCreator.h \
		src/UnknownRecordListModel.h \
		src/UrlPage.h \
		src/Util.h \
		src/VCardNdefRecord.h \
		src/ViewHeader.h \
		src/WritePage.h

SOURCES		+= \
		src/Tagbuilder.cpp \
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
		src/CalendarDetailPicker.cpp \
		src/CalendarDetailPickerListCellCreator.cpp \
		src/CalendarDetailPickerListModel.cpp \
		src/CalendarPage.cpp \
		src/CalendarSelectionPage.cpp \
		src/CalendarSelectionPageListCellCreator.cpp \
		src/CalendarSelectionPageListModel.cpp \
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
		src/FoursquareAuthNetworkAccessManager.cpp \
		src/FoursquareAuthPage.cpp \
		src/FoursquareAuthStorage.cpp \
		src/FoursquareVenue.cpp \
		src/FoursquareVenueSearch.cpp \
		src/FoursquareVenueSelectionPage.cpp \
		src/FoursquareVenueSelectionPageListCellCreator.cpp \
		src/FoursquareVenueSelectionPageListModel.cpp \
		src/HelpPage.cpp \
		src/LabeledTextEdit.cpp \
		src/LabelOrList.cpp \
		src/MainPage.cpp \
		src/NfcdMonitor.cpp \
		src/NfcHandoverSelectNdefRecord.cpp \
		src/OrganizerItemHandler.cpp \
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
		src/UnknownRecordListCellCreator.cpp \
		src/UnknownRecordListModel.cpp \
		src/UrlPage.cpp \
		src/Util.cpp \
		src/VCardNdefRecord.cpp \
		src/ViewHeader.cpp \
		src/WritePage.cpp

TRANSLATIONS	= \
		Tagbuilder_en.ts \
		Tagbuilder_fi.ts

target.path	= $$PREFIX/bin

gfx.path	= $$PREFIX/share/images
gfx.files	= \
		data/Tagbuilder.png \
		data/tap.png

trans.path	= $$PREFIX/share/l10n/Tagbuilder
trans.files	= \
		Tagbuilder_en.qm \
		Tagbuilder_fi.qm

desktop.path	= /usr/share/applications
desktop.files	= \
		data/Tagbuilder.desktop

style.path	= /usr/share/themes/base/meegotouch/Tagbuilder/style
style.files	= \
		data/Tagbuilder.css

INSTALLS	+= target gfx trans desktop style

unix {
        CONFIG += link_pkgconfig
        PKGCONFIG += contentaction-0.1 QJson
}
