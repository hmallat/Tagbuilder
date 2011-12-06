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
#include "LabeledTextEdit.h"
#include "TagStorage.h"
#include "Tag.h"

#include <MAction>
#include <MLabel>
#include <MButton>
#include <MMessageBox>
#include <MContentItem>
#include <QGraphicsAnchorLayout>
#include <QGraphicsLinearLayout>
#include <QContactLocalIdFilter>
#include <QVersitOrganizerExporter>
#include <QVersitOrganizerImporter>
#include <QVersitReader>
#include <QVersitWriter>

#include <MDebug>

CalendarPage::CalendarPage(int tag, QGraphicsItem *parent)
	: MApplicationPage(parent),
	  m_tag(tag),
	  m_name(0),
	  m_calendar(0),
	  m_cancelAction(0),
	  m_storeAction(0),
	  m_info()
{
	setComponentsDisplayMode(MApplicationPage::EscapeButton,
				 MApplicationPageModel::Hide);
}

CalendarPage::~CalendarPage(void)
{
}

void CalendarPage::createContent(void)
{
	m_cancelAction = new MAction(tr("Cancel"), this);
	m_cancelAction->setLocation(MAction::ToolBarLocation);
	connect(m_cancelAction, SIGNAL(triggered()),
		this, SLOT(dismiss()));
	addAction(m_cancelAction);

	m_storeAction = new MAction(tr("Store"), this);
	if (m_tag == -1) {
		m_storeAction->setEnabled(false);
	}
	m_storeAction->setLocation(MAction::ToolBarLocation);
	connect(m_storeAction, SIGNAL(triggered()),
		this, SLOT(storeTag()));
	addAction(m_storeAction);

	QGraphicsLinearLayout *layout = 
		new QGraphicsLinearLayout(Qt::Vertical);

	MLabel *label = new MLabel(m_tag == -1 
				   ? tr("<big>Create tag contents</big>")
				   : tr("<big>Edit tag contents</big>"));
	label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	label->setAlignment(Qt::AlignCenter);
	layout->addItem(label);
	layout->setAlignment(label, Qt::AlignCenter);

	m_name = new LabeledTextEdit(MTextEditModel::SingleLine,
				     tr("Tag name"),
				     tr("Enter tag name"));
	if (m_tag != -1) {
		m_name->setText(TagStorage::tag(m_tag)->name());
	}
	m_name->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	layout->addItem(m_name);
	layout->setAlignment(m_name, Qt::AlignLeft);
	connect(m_name, SIGNAL(textChanged(void)),
		this, SLOT(nameChanged(void)));

	MLabel *selected = new MLabel(tr("Selected calendar entry"));
	selected->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	selected->setAlignment(Qt::AlignLeft);
	layout->addItem(selected);
	layout->setAlignment(selected, Qt::AlignLeft);

	m_calendar = new MContentItem();
	m_calendar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	if (m_tag != -1) {
		importCalendarItem();
	} else {
		setCalendarItem(QOrganizerItem());
	}
	layout->addItem(m_calendar);
	layout->setAlignment(m_calendar, Qt::AlignCenter);

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
	
		layout->addItem(sub_layout);
		layout->setAlignment(sub_layout, Qt::AlignCenter);
	}
	
	centralWidget()->setLayout(layout);
}

void CalendarPage::nameChanged(void)
{
	if (m_name->text() == "") {
		/* cannot store without a name */
		m_storeAction->setEnabled(false);
	} else {
		m_storeAction->setEnabled(true);
	}
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
}

void CalendarPage::storeTag(void)
{
	bool success;
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

	if (m_tag == -1) {
		success = TagStorage::append(m_name->text(), message);
	} else {
		success = TagStorage::update(m_tag,
					     m_name->text(),
					     message);
	}

	if (success == false) {
		goto fail;
	}

	dismiss();
	return;

fail:
	MMessageBox *box = new MMessageBox(tr("Cannot store the tag. "));
	box->appear();
}

void CalendarPage::importCalendarItem(void)
{
	QNdefRecord r = TagStorage::tag(m_tag)->message().at(0);
	VCalNdefRecord vc(TagStorage::tag(m_tag)->message().at(0));
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
	return;

fail:
	setCalendarItem(QOrganizerItem());
	MMessageBox *box = new MMessageBox(tr("Cannot read the tag. "));
	box->appear();
}
