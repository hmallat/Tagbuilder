/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "CalendarSelectionPageListModel.h"

#include <MDebug>

CalendarSelectionPageListModel::
CalendarSelectionPageListModel(QOrganizerManager *manager,
			       QObject *parent)
	: MAbstractItemModel(parent),
	  m_manager(manager),
	  m_fetch(0),
	  m_items()
{
	setGrouped(true);

	/* TODO */
}

void CalendarSelectionPageListModel::resultsAvailable(void)
{
	/* TODO */
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
