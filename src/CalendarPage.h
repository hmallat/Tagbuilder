/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _CALENDAR_PAGE_H
#define _CALENDAR_PAGE_H_

#include "CreateEditPage.h"

#include <QOrganizerItem>
#include <QOrganizerManager>

class MList;
class MLabel;

QTM_USE_NAMESPACE;

class CalendarPage : public CreateEditPage
{

	Q_OBJECT;

public:

	CalendarPage(int tag = TagStorage::NULL_TAG, 
		     QGraphicsItem *parent = 0);

	virtual ~CalendarPage(void);

protected:

	virtual void createPageSpecificActions(void);

	virtual void createPageSpecificContent(void);

	virtual void setupNewData(void);

	virtual bool setupData(const QNdefMessage message);

	virtual QNdefMessage prepareDataForStorage(void);

private Q_SLOTS:

	void chooseEvent(void);

	void chooseTodo(void);

	void setCalendarItemThroughAction(const QOrganizerItem item);

	void selectCalendarDetails(const QOrganizerItem item);

private:

	Q_DISABLE_COPY(CalendarPage);

	void updateSize(void);

	void setCalendarItem(const QOrganizerItem item);

	QOrganizerManager m_calendarManager;

	QOrganizerItem m_info;

	MLabel *m_calendarTitle;

	MList *m_calendarDetails;

};

#endif /* _CALENDAR_PAGE_H_ */
