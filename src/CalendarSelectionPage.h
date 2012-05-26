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
