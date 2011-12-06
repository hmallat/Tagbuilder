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
#include "VCalNdefRecord.h"

#include <MLabel>
#include <MButton>
#include <MContentItem>
#include <QGraphicsLinearLayout>
#include <QVersitOrganizerExporter>
#include <QVersitOrganizerImporter>
#include <QVersitReader>
#include <QVersitWriter>

#include <MDebug>

CalendarPage::CalendarPage(int tag, QGraphicsItem *parent)
	: CreateEditPage(tag, parent),
	  m_calendar(0),
	  m_info()
{
}

CalendarPage::~CalendarPage(void)
{
}

void CalendarPage::createPageSpecificContent(void)
{
	MLabel *selected = new MLabel(tr("Selected calendar entry"));
	selected->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	selected->setAlignment(Qt::AlignLeft);
	layout()->addItem(selected);
	layout()->setAlignment(selected, Qt::AlignLeft);

	m_calendar = new MContentItem();
	m_calendar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	layout()->addItem(m_calendar);
	layout()->setAlignment(m_calendar, Qt::AlignCenter);

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
		goto fail;
	}

	if (writer.startWriting(exporter.document()) == false ||
	    writer.waitForFinished() == false) {
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
		this, SLOT(setCalendarItem(const QOrganizerItem)));
}

void CalendarPage::chooseTodo(void)
{
	CalendarSelectionPageListModel::ListType type = 
		CalendarSelectionPageListModel::ListTodos;
	CalendarSelectionPage *page = 
		new CalendarSelectionPage(type, &m_calendarManager);
	page->appear(scene(), MSceneWindow::DestroyWhenDismissed);
	connect(page, SIGNAL(selected(const QOrganizerItem)),
		this, SLOT(setCalendarItem(const QOrganizerItem)));
}

void CalendarPage::setCalendarItem(const QOrganizerItem item)
{
	m_info = item;
	m_calendar->setImageID("icon-m-content-calendar"); 
	m_calendar->setTitle(m_info.isEmpty() 
			     ? "No entry selected"
			     : m_info.displayLabel());
	m_calendar->setSubtitle(m_info.isEmpty()
				? ""
				: "Insert time here");
	setContentValidity(m_info.isEmpty() ? false : true);
}
