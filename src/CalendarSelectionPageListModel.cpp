/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "CalendarSelectionPageListModel.h"

#include <QMap>
#include <QOrganizerItemDetailFilter>
#include <QOrganizerItemUnionFilter>
#include <QOrganizerItemType>
#include <QOrganizerEvent>
#include <QOrganizerEventOccurrence>
#include <QOrganizerTodo>
#include <QOrganizerTodoOccurrence>

#include <MDebug>

CalendarSelectionPageListModel::
CalendarSelectionPageListModel(CalendarSelectionPageListModel::ListType type,
			       QOrganizerManager *manager,
			       QObject *parent)
	: MAbstractItemModel(parent),
	  m_manager(manager),
	  m_fetch(0),
	  m_items()
{
	setGrouped(true);

	QOrganizerItemDetailFilter eventFilter;
	eventFilter.setDetailDefinitionName(QOrganizerItemType::DefinitionName,
					    QOrganizerItemType::FieldType);
	if (type == ListEvents) {
		eventFilter.setValue(QOrganizerItemType::TypeEvent);
	} else {
		eventFilter.setValue(QOrganizerItemType::TypeTodo);
	}

	QOrganizerItemDetailFilter occurFilter;
	occurFilter.setDetailDefinitionName(QOrganizerItemType::DefinitionName,
					    QOrganizerItemType::FieldType);
	if (type == ListEvents) {
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
		/* TODO: indicate in UI */
	}
}

void CalendarSelectionPageListModel::resultsAvailable(void)
{
	if (m_fetch->error() != QOrganizerManager::NoError) {
		mDebug(__func__) << "Cannot fetch calendar entries, error " 
				 << m_fetch->error();
		/* TODO: indicate in UI */
		return;
	}

	QMap <QDate, QList<QOrganizerItem> > insertions;

	QList<QOrganizerItem> results = m_fetch->items();
	for (int i = 0; i < results.length(); i++) {
		QDate bucket;

		if (results[i].type() == QOrganizerItemType::TypeEvent) {
			bucket = static_cast<QOrganizerEvent>(results[i]).startDateTime().date();
		} else if (results[i].type() == 
			   QOrganizerItemType::TypeEventOccurrence) {
			bucket = static_cast<QOrganizerEventOccurrence>(results[i]).startDateTime().date();
		} else if (results[i].type() == QOrganizerItemType::TypeTodo) {
			bucket = static_cast<QOrganizerTodo>(results[i]).startDateTime().date();
		} else if (results[i].type() == 
			   QOrganizerItemType::TypeTodoOccurrence) {
			bucket = static_cast<QOrganizerTodoOccurrence>(results[i]).startDateTime().date();
		} 

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
		/* TODO: indicate in UI */
		return;
	}

	Q_EMIT(layoutAboutToBeChanged());
	QList<QDate> keys = insertions.keys();
	beginInsertRows(QModelIndex(), 0, keys.length() - 1, false);

	/* TODO: check the date() for non-dated todos */

	/* TODO: can the list be scrolled to current date ? */

	for (int i = 0; i < keys.length(); i++) {
		QList<QOrganizerItem> items = insertions[keys[i]];
		/* TODO: sort items by start time! */
		QPair<QDate, QList<QOrganizerItem> > pair(keys[i], items);
		m_items << pair;
	}

	endInsertRows();
	Q_EMIT(layoutChanged());
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
		parameters 
			<< item.displayLabel()
			<< "mumble mumble mumble"
			<< "icon-m-content-calendar";

	return qVariantFromValue(parameters);
}

QOrganizerItem CalendarSelectionPageListModel::calendarItem(const QModelIndex &index) const
{
	int row = index.row();
	int group = index.parent().row();
	return m_items[group].second[row];
}
