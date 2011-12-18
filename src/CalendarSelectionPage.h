/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _CALENDAR_SELECTION_PAGE_H_
#define _CALENDAR_SELECTION_PAGE_H_

#include "SelectionPage.h"
#include "CalendarSelectionPageListModel.h"

#include <QOrganizerManager>
#include <QOrganizerItem>

QTM_USE_NAMESPACE;

class CalendarSelectionPage : public SelectionPage
{
	Q_OBJECT;

public:

	CalendarSelectionPage(enum CalendarSelectionPageListModel::ListType type,
			      QOrganizerManager *manager,
			      QGraphicsItem *parent = 0);

	virtual ~CalendarSelectionPage(void);

        virtual void createContent(void);

signals:

	void selected(const QOrganizerItem which);

private Q_SLOTS:
	
	void calendarItemSelected(const QModelIndex &which);

	void itemsReady(void);

private:

	Q_DISABLE_COPY(CalendarSelectionPage);

	CalendarSelectionPageListModel *m_model;

};

#endif /* _CALENDAR_SELECTION_PAGE_H_ */
