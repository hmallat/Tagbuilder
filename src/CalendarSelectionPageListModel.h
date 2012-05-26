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

	enum ListType {
		ListEvents,
		ListTodos
	};

	CalendarSelectionPageListModel(enum ListType type,
				       QOrganizerManager *manager,
				       QObject *parent = 0);

	virtual int groupCount(void) const;

	virtual int rowCountInGroup(int group) const;

	virtual QString groupTitle(int group) const;

	virtual QVariant itemData(int row, 
				  int group, 
				  int role = Qt::DisplayRole) const;

	QOrganizerItem calendarItem(const QModelIndex &index) const;

	QModelIndex groupClosestToNow(void) const;

	bool fetch(void);

Q_SIGNALS:

	void ready(void);

protected Q_SLOTS:

	void resultsAvailable(void);

	void stateChanged(QOrganizerAbstractRequest::State);

protected:

	enum ListType m_type;

	QOrganizerManager *m_manager;

	QOrganizerItemFetchRequest *m_fetch;

	QList< QPair< QDate, QList< QOrganizerItem > > > m_items;

	QModelIndex m_closest;

};

#endif /* _CALENDAR_SELECTION_PAGE_LIST_MODEL_H_ */
