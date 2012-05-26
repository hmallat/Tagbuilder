/*

    NFC Tag Builder for Nokia N9
    Copyright (C) 2011,2012  Hannu Mallat <hmallat@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
#include <MButton>
#include <QGraphicsLinearLayout>

#include <MDebug>

static MAbstractCellCreator<MWidgetController> *_getCreator(void)
{
	return new CalendarDetailPickerListCellCreator("UnselectableContentItem");
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
	QList<MButton *> buttons;

	MButton *eButton = new MButton();
	eButton->setText(tr("Calendar event"));
	eButton->setSizePolicy(QSizePolicy::Preferred, 
			       QSizePolicy::Fixed);
	connect(eButton, SIGNAL(clicked()), this, SLOT(chooseEvent()));
	buttons << eButton;

	MButton *tButton = new MButton();
	tButton->setText(tr("Calendar to-do item"));
	tButton->setSizePolicy(QSizePolicy::Preferred, 
			       QSizePolicy::Fixed);
	connect(tButton, SIGNAL(clicked()), this, SLOT(chooseTodo()));
	buttons << tButton;

	m_calendarDetails = new LabelOrList(m_model,
					    _getCreator,
					    tr("Select a calendar entry"),
					    true,
					    false,
					    buttons);
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
	QOrganizerItem o = Util::organizerItemFromNdef(message);
	if (o.isEmpty() == true) {
		setCalendarItem(QOrganizerItem());
		return false;
	}

	setCalendarItem(o);
	if (isTemporary() == true) {
		setDefaultName(m_info.displayLabel());
	}
	return true;
}

QNdefMessage CalendarPage::prepareDataForStorage(void)
{
	return Util::ndefFromOrganizerItem(m_info);
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
#ifdef LABEL_SIZE
	updateSize();
#endif
}

void CalendarPage::setCalendarItemThroughAction(const QOrganizerItem item)
{
	setCalendarItem(item);
	setDefaultName(item.displayLabel());
}

#ifdef LABEL_SIZE
void CalendarPage::updateSize(void)
{
	/* No easy way to calculate the size of the exported VCAL file,
	   so make a message out of it... although that can get slow
	   when typing in the title! */
	QNdefMessage message = prepareDataForStorage();
	quint32 ndefLength = Util::messageLength(message);
	setContentSize(ndefLength);
}
#endif
