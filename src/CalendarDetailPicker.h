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
