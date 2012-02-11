/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "CalendarSelectionPageListModel.h"
#include "Util.h"

#include <QMap>
#include <QOrganizerItemDetailFilter>
#include <QOrganizerItemUnionFilter>
#include <QOrganizerItemType>
#include <QOrganizerEvent>
#include <QOrganizerEventOccurrence>
#include <QOrganizerTodo>
#include <QOrganizerTodoOccurrence>

#include <MDebug>

static bool _itemAllDay(const QOrganizerItem &i)
{
	if (i.type() == QOrganizerItemType::TypeEvent) {
		return static_cast<QOrganizerEvent>(i).isAllDay();
	} else if (i.type() == QOrganizerItemType::TypeEventOccurrence) {
		return false; /* TODO should check parent! */
	} else if (i.type() == QOrganizerItemType::TypeTodo) {
		return static_cast<QOrganizerTodo>(i).isAllDay();
	} else if (i.type() == QOrganizerItemType::TypeTodoOccurrence) {
		return false; /* TODO should check parent! */
	} else {
		return false;
	}
}
static QDateTime _itemStart(const QOrganizerItem &i)
{
	if (i.type() == QOrganizerItemType::TypeEvent) {
		return static_cast<QOrganizerEvent>(i).startDateTime();
	} else if (i.type() == QOrganizerItemType::TypeEventOccurrence) {
		return static_cast<QOrganizerEventOccurrence>(i).startDateTime();
	} else if (i.type() == QOrganizerItemType::TypeTodo) {
		return static_cast<QOrganizerTodo>(i).startDateTime();
	} else if (i.type() == QOrganizerItemType::TypeTodoOccurrence) {
		return static_cast<QOrganizerTodoOccurrence>(i).startDateTime();
	} else {
		return QDateTime();
	}
}

static QDateTime _itemEnd(const QOrganizerItem &i)
{
	if (i.type() == QOrganizerItemType::TypeEvent) {
		return static_cast<QOrganizerEvent>(i).endDateTime();
	} else if (i.type() == QOrganizerItemType::TypeEventOccurrence) {
		return static_cast<QOrganizerEventOccurrence>(i).endDateTime();
	} else if (i.type() == QOrganizerItemType::TypeTodo) {
		return QDateTime();
	} else if (i.type() == QOrganizerItemType::TypeTodoOccurrence) {
		return QDateTime();
	} else {
		return QDateTime();
	}
}

bool operator< (const QOrganizerItem &i1, const QOrganizerItem &i2)
{
	return _itemStart(i1) < _itemStart(i2);
}

CalendarSelectionPageListModel::
CalendarSelectionPageListModel(CalendarSelectionPageListModel::ListType type,
			       QOrganizerManager *manager,
			       QObject *parent)
	: MAbstractItemModel(parent),
	  m_type(type),
	  m_manager(manager),
	  m_fetch(0),
	  m_items(),
	  m_closest()
{
	setGrouped(true);
}

bool CalendarSelectionPageListModel::fetch(void)
{
	if (m_fetch != NULL) {
		delete m_fetch;
		m_fetch = NULL;
	}

	QOrganizerItemDetailFilter eventFilter;
	eventFilter.setDetailDefinitionName(QOrganizerItemType::DefinitionName,
					    QOrganizerItemType::FieldType);
	if (m_type == ListEvents) {
		eventFilter.setValue(QOrganizerItemType::TypeEvent);
	} else {
		eventFilter.setValue(QOrganizerItemType::TypeTodo);
	}

	QOrganizerItemDetailFilter occurFilter;
	occurFilter.setDetailDefinitionName(QOrganizerItemType::DefinitionName,
					    QOrganizerItemType::FieldType);
	if (m_type == ListEvents) {
		occurFilter.setValue(QOrganizerItemType::TypeEventOccurrence);
	} else {
		occurFilter.setValue(QOrganizerItemType::TypeTodoOccurrence);
	}

	QOrganizerItemUnionFilter unionFilter;
	unionFilter << eventFilter;
	unionFilter << occurFilter;

	m_fetch = new QOrganizerItemFetchRequest(this);
	m_fetch->setManager(m_manager);
	m_fetch->setFilter(unionFilter);

	connect(m_fetch, SIGNAL(resultsAvailable()),
		this, SLOT(resultsAvailable()));

	mDebug(__func__) << "Starting fetch. ";
	if (m_fetch->start() == false) {
		mDebug(__func__) << "Cannot fetch contacts. ";
		return false;
	}

	return true;
}

void CalendarSelectionPageListModel::resultsAvailable(void)
{
	QMap <QDate, QList<QOrganizerItem> > insertions;
	QList<QOrganizerItem> results;

	if (m_fetch->error() != QOrganizerManager::NoError) {
		mDebug(__func__) << "Cannot fetch calendar entries, error " 
				 << m_fetch->error();
		goto done;
	}

	results = m_fetch->items();
	for (int i = 0; i < results.length(); i++) {
		QDate bucket = _itemStart(results[i]).date();

		if (insertions.contains(bucket)) {
			insertions[bucket] << results[i];
		} else {
			QList<QOrganizerItem> list;
			list << results[i];
			insertions[bucket] = list;
		}
	}

	if (insertions.size() == 0) {
		mDebug(__func__) << "Nothing to be inserted. ";
		goto done;
	}

done:
	Q_EMIT(layoutAboutToBeChanged());

	if (m_items.length() > 0) {
		beginRemoveRows(QModelIndex(), 0, m_items.length() - 1, 
				Util::animateLists);
		m_items.clear();
		endRemoveRows();
	}


	QList<QDate> keys = insertions.keys();
	if (keys.length() != 0) {
		QList< QPair <QDate, QList< QOrganizerItem > > > pairs;

		beginInsertRows(QModelIndex(), 0, keys.length() - 1, 
				Util::animateLists);

		/* TODO: check the date() for non-dated todos */

		QDate now = QDate::currentDate();
		int closestToNow = -1;
		int daysToClosest = 0;

		for (int i = 0; i < keys.length(); i++) {
			QList<QOrganizerItem> items = insertions[keys[i]];
			qSort(items);
			QPair<QDate, QList<QOrganizerItem> > pair(keys[i], items);
			pairs << pair;
		
			if (closestToNow == -1) {
				closestToNow = i;
				daysToClosest = qAbs(now.daysTo(keys[i]));
			} else {
				int delta = qAbs(now.daysTo(keys[i]));
				if (delta < daysToClosest) {
					closestToNow = i;
					daysToClosest = delta;
				}
			}
		}
		
		m_items = pairs;

		mDebug(__func__) << "Closest to now is group " << closestToNow;

		QModelIndex groupIndex = index(closestToNow, 0);
		m_closest = groupIndex;
		
		endInsertRows();

	} else {
		m_closest = QModelIndex();
	}

	Q_EMIT(layoutChanged());

	Q_EMIT(ready());
}

void CalendarSelectionPageListModel::stateChanged(QOrganizerAbstractRequest::State)
{
	/* TODO */
}

int CalendarSelectionPageListModel::groupCount(void) const
{
	return m_items.length();
}

int CalendarSelectionPageListModel::rowCountInGroup(int group) const
{
	return m_items[group].second.length();
}

QString CalendarSelectionPageListModel::groupTitle(int group) const
{
	return m_items[group].first.toString();
}

QVariant CalendarSelectionPageListModel::itemData(int row, 
						  int group, 
						  int role) const
{
	(void)role;

	QStringList parameters;
	QOrganizerItem item = m_items[group].second[row];

	parameters << item.displayLabel();

	/* TODO: handle those which don't have a valid timestamp */
	bool a = _itemAllDay(item);
	QDateTime s = _itemStart(item);
	QDateTime e = _itemEnd(item);
	QString stamp;

	if (a == true) {
		/* Time component is meaningless */
		stamp = tr("All-day event");
	} else {
		stamp = Util::eventTimeToString(s, e);
	}
	parameters << stamp;

	parameters << "icon-m-content-calendar";

	return qVariantFromValue(parameters);
}

QOrganizerItem CalendarSelectionPageListModel::calendarItem(const QModelIndex &index) const
{
	mDebug(__func__) 
		<< "Index(" << index.column() << "," << index.row() << ")";
	mDebug(__func__) 
		<< "Parent(" << index.parent().column() << "," << index.parent().row() << ")";
		
	int row = index.row();
	int group = index.parent().row();
	return m_items[group].second[row];
}

QModelIndex CalendarSelectionPageListModel::groupClosestToNow(void) const
{
	return m_closest;
}

