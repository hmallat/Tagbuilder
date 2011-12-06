/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _CALENDAR_PAGE_H
#define _CALENDAR_PAGE_H_

#include <MApplicationPage>
#include <QOrganizerItem>
#include <QOrganizerManager>

class LabeledTextEdit;
class MContentItem;
class MButton;
class MAction;

QTM_USE_NAMESPACE;

class CalendarPage : public MApplicationPage
{

	Q_OBJECT;

public:

	CalendarPage(int tag = -1, 
		     QGraphicsItem *parent = 0);

	~CalendarPage(void);

	virtual void createContent(void);

private Q_SLOTS:

	void nameChanged(void);

	void storeTag(void);

	void chooseEvent(void);

	void chooseTodo(void);

	void setCalendarItem(const QOrganizerItem item);

private:

	Q_DISABLE_COPY(CalendarPage);

	void importCalendarItem(void);

	int m_tag;

	LabeledTextEdit *m_name;

	MContentItem *m_calendar;

	MAction *m_cancelAction;

	MAction *m_storeAction;

	QOrganizerManager m_calendarManager;

	QOrganizerItem m_info;

};

#endif /* _CALENDAR_PAGE_H_ */
