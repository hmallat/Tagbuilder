/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _CALENDAR_SELECTION_PAGE_LIST_MODEL_H_
#define _CALENDAR_SELECTION_PAGE_LIST_MODEL_H_

#include <MAbstractItemModel>
#include <QOrganizerManager>
#include <QOrganizerItem>
#include <QOrganizerItemFetchRequest>
#include <QMap>
#include <QDateTime>

QTM_USE_NAMESPACE;

class CalendarSelectionPageListModel : public MAbstractItemModel
{

	Q_OBJECT;

public:

	CalendarSelectionPageListModel(QOrganizerManager *manager,
				       QObject *parent = 0);

	virtual int groupCount(void) const;

	virtual int rowCountInGroup(int group) const;

	virtual QString groupTitle(int group) const;

	virtual QVariant itemData(int row, 
				  int group, 
				  int role = Qt::DisplayRole) const;

	QOrganizerItem calendarItem(const QModelIndex &index) const;

protected Q_SLOTS:

	void resultsAvailable(void);

	void stateChanged(QOrganizerAbstractRequest::State);

protected:

	QOrganizerManager *m_manager;

	QOrganizerItemFetchRequest *m_fetch;

	QList< QPair< QDateTime, QList< QOrganizerItem > > > m_items;

};

#endif /* _CALENDAR_SELECTION_PAGE_LIST_MODEL_H_ */
