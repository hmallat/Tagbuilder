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
#include "LabelOrList.h"
#include "VCalNdefRecord.h"
#include "Util.h"

#include <MLabel>
#include <MAction>
#include <QGraphicsLinearLayout>
#include <QVersitOrganizerExporter>
#include <QVersitOrganizerImporter>
#include <QVersitReader>
#include <QVersitWriter>

#include <MDebug>

static MAbstractCellCreator<MWidgetController> *_getCreator(void)
{
	return new CalendarDetailPickerListCellCreator;
}

CalendarPage::CalendarPage(int tag, QGraphicsItem *parent)
	: CreateEditPage(tag, parent),
	  m_info(),
	  m_calendarDetails(0),
	  m_model(new CalendarDetailPickerListModel(this))
{
}

CalendarPage::~CalendarPage(void)
{
}

void CalendarPage::createPageSpecificActions(void)
{
	MAction *eventAction = new MAction(tr("Event from calendar..."),
					   this);
	eventAction->setLocation(MAction::ApplicationMenuLocation);
	connect(eventAction, SIGNAL(triggered()),
		this, SLOT(chooseEvent()));
	addAction(eventAction);
	
	MAction *todoAction = new MAction(tr("Todo item from calendar..."),
					  this);
	todoAction->setLocation(MAction::ApplicationMenuLocation);
	connect(todoAction, SIGNAL(triggered()),
		this, SLOT(chooseTodo()));
	addAction(todoAction);
	
}

void CalendarPage::createPageSpecificContent(void)
{
	m_calendarDetails = new LabelOrList(m_model,
					    _getCreator,
					    tr("Select a calendar entry"),
					    true,
					    false);
	m_calendarDetails->setSizePolicy(QSizePolicy::Preferred, 
					 QSizePolicy::Preferred);
	layout()->addItem(m_calendarDetails);
	layout()->setAlignment(m_calendarDetails, Qt::AlignCenter);
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
		this, SLOT(setCalendarItemThroughAction(const QOrganizerItem)));
}

void CalendarPage::setCalendarItem(const QOrganizerItem item)
{
	m_info = item;

	m_model->setOrganizerItem(m_info);

	setContentValidity(m_info.isEmpty() ? false : true);
	updateSize();
}

void CalendarPage::setCalendarItemThroughAction(const QOrganizerItem item)
{
	setCalendarItem(item);
	setDefaultName(item.displayLabel());
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
