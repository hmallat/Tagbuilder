/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _CALENDAR_DETAIL_PICKER_H_
#define _CALENDAR_DETAIL_PICKER_H_

#include "SelectionPage.h"

#include <QOrganizerItem>

QTM_USE_NAMESPACE;

class LabelOrList;
class MAction;
class QGraphicsLinearLayout;
class CalendarDetailPickerListModel;
class QModelIndex;

class CalendarDetailPicker : public SelectionPage
{

	Q_OBJECT;

public:

	CalendarDetailPicker(const QOrganizerItem item, 
			     QGraphicsItem *parent = 0);

	virtual ~CalendarDetailPicker(void);

        virtual void createContent(void);

Q_SIGNALS:

	void calendarPicked(const QOrganizerItem item);

private Q_SLOTS:

	void pickingDone(void);

private:

	Q_DISABLE_COPY(CalendarDetailPicker);

	QOrganizerItem m_item;

	CalendarDetailPickerListModel *m_model;

};

#endif /* _CALENDAR_DETAIL_PICKER_H_ */
