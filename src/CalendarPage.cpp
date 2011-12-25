/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "CalendarPage.h"
#include "CalendarSelectionPage.h"
#include "CalendarSelectionPageListModel.h"
#include "CalendarDetailPicker.h"
#include "CalendarDetailPickerListCellCreator.h"
#include "CalendarDetailPickerListModel.h"
#include "VCalNdefRecord.h"
#include "Util.h"

#include <MLabel>
#include <MList>
#include <MSeparator>
#include <MButton>
#include <QGraphicsLinearLayout>
#include <QVersitOrganizerExporter>
#include <QVersitOrganizerImporter>
#include <QVersitReader>
#include <QVersitWriter>

#include <MDebug>

CalendarPage::CalendarPage(int tag, QGraphicsItem *parent)
	: CreateEditPage(tag, parent),
	  m_info(),
	  m_calendarTitle(0),
	  m_calendarDetails(0)
{
}

CalendarPage::~CalendarPage(void)
{
}

void CalendarPage::createPageSpecificContent(void)
{
	m_calendarTitle = new MLabel();
	m_calendarTitle->setSizePolicy(QSizePolicy::Minimum, 
				       QSizePolicy::Fixed);
	m_calendarTitle->setAlignment(Qt::AlignLeft);
	m_calendarTitle->setWordWrap(true);
	layout()->addItem(m_calendarTitle);
	layout()->setAlignment(m_calendarTitle, Qt::AlignLeft);

	m_calendarDetails = new MList();
	m_calendarDetails->setShowGroups(true);
	CalendarDetailPickerListCellCreator *creator = 
		new CalendarDetailPickerListCellCreator;
	m_calendarDetails->setCellCreator(creator);
	m_calendarDetails->setSizePolicy(QSizePolicy::Preferred, 
					 QSizePolicy::Preferred);
	layout()->addItem(m_calendarDetails);
	layout()->setAlignment(m_calendarDetails, Qt::AlignCenter);

	MSeparator *sep = new MSeparator;
	sep->setStyleName("CommonHorizontalSeparator");
	sep->setOrientation(Qt::Horizontal);
	layout()->addItem(sep);

	{
		QGraphicsLinearLayout *sub_layout = 
			new QGraphicsLinearLayout(Qt::Vertical);

		MButton *event_button = 
			new MButton(tr("Choose an event"));
		sub_layout->addItem(event_button);
		connect(event_button, SIGNAL(clicked()),
			this, SLOT(chooseEvent()));
	
		MButton *todo_button = 
			new MButton(tr("Choose a to-do item"));
		sub_layout->addItem(todo_button);
		connect(todo_button, SIGNAL(clicked()),
			this, SLOT(chooseTodo()));
	
		layout()->addItem(sub_layout);
		layout()->setAlignment(sub_layout, Qt::AlignCenter);
	}
	
}

void CalendarPage::setupNewData(void) 
{
	setCalendarItem(QOrganizerItem());
}

bool CalendarPage::setupData(const QNdefMessage message)
{
	VCalNdefRecord vc(message[0]);
	QVersitReader reader(vc.payload());
	QVersitOrganizerImporter importer;

	if (reader.startReading() == false ||
	    reader.waitForFinished() == false) {
		mDebug(__func__) << "Reader fail, " << reader.error() << ". ";
		goto fail;
	}

	if (reader.results().length() == 0) {
		mDebug(__func__) << "No results. ";
		goto fail;
	}

	if (importer.importDocument(reader.results()[0]) == false ||
	    importer.items().length() == 0) {
		mDebug(__func__) << "Importer fail. ";
		goto fail;
	}

	setCalendarItem(importer.items()[0]);
	return true;

fail:
	return false;
}

QNdefMessage CalendarPage::prepareDataForStorage(void)
{
	QNdefMessage message;
	VCalNdefRecord vc;
	QByteArray data;
	QVersitWriter writer(&data);
	QVersitOrganizerExporter exporter;

	if (exporter.exportItems(QList<QOrganizerItem>() << m_info,
				 QVersitDocument::ICalendar20Type) == false) {
		mDebug(__func__) << "Export fail";
		goto fail;
	}

	if (writer.startWriting(exporter.document()) == false ||
	    writer.waitForFinished() == false) {
		mDebug(__func__) << "Write fail";
		goto fail;
	}

	vc.setPayload(data);
	message << vc;

	return message;

fail:
	return QNdefMessage();
}

void CalendarPage::chooseEvent(void)
{
	CalendarSelectionPageListModel::ListType type = 
		CalendarSelectionPageListModel::ListEvents;
	CalendarSelectionPage *page = 
		new CalendarSelectionPage(type, &m_calendarManager);
	page->appear(scene(), MSceneWindow::DestroyWhenDismissed);
	connect(page, SIGNAL(selected(const QOrganizerItem)),
		this, SLOT(selectCalendarDetails(const QOrganizerItem)));
}

void CalendarPage::chooseTodo(void)
{
	CalendarSelectionPageListModel::ListType type = 
		CalendarSelectionPageListModel::ListTodos;
	CalendarSelectionPage *page = 
		new CalendarSelectionPage(type, &m_calendarManager);
	page->appear(scene(), MSceneWindow::DestroyWhenDismissed);
	connect(page, SIGNAL(selected(const QOrganizerItem)),
		this, SLOT(selectCalendarDetails(const QOrganizerItem)));
}

void CalendarPage::selectCalendarDetails(const QOrganizerItem item)
{
	CalendarDetailPicker *picker = new CalendarDetailPicker(item);
	picker->appear(MSceneWindow::DestroyWhenDismissed);
	connect(picker, SIGNAL(calendarPicked(const QOrganizerItem)),
		this, SLOT(setCalendarItem(const QOrganizerItem)));
}

void CalendarPage::setCalendarItem(const QOrganizerItem item)
{
	m_info = item;

	if (m_info.isEmpty() == true) {
		m_calendarTitle->setText(tr("No calendar entry selected"));
	} else {
		m_calendarTitle->setText(tr("Calendar entry details"));
	}
	CalendarDetailPickerListModel *model = 
		new CalendarDetailPickerListModel(m_info,
						  m_calendarDetails);
	m_calendarDetails->setItemModel(model);

	setContentValidity(m_info.isEmpty() ? false : true);
	updateSize();
}

void CalendarPage::updateSize(void)
{
	/* No easy way to calculate the size of the exported VCAL file,
	   so make a message out of it... although that can get slow
	   when typing in the title! */
	QNdefMessage message = prepareDataForStorage();
	quint32 ndefLength = Util::messageLength(message);
	setContentSize(ndefLength);
}
