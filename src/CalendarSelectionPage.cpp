/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "CalendarSelectionPage.h"
#include "CalendarSelectionPageListCellCreator.h"
#include "CalendarSelectionPageListModel.h"

#include <MList>

#include <MDebug>

CalendarSelectionPage::CalendarSelectionPage(CalendarSelectionPageListModel::ListType type,
					     QOrganizerManager *manager,
					     QGraphicsItem *parent)
	: SelectionPage(parent),
	  m_model(new CalendarSelectionPageListModel(type, manager, this))
{
}

CalendarSelectionPage::~CalendarSelectionPage(void)
{
}

void CalendarSelectionPage::createContent(void)
{
	createCommonContent(tr("<big>Select the calendar entry to use</big>"),
			    true);

	CalendarSelectionPageListCellCreator *creator = 
		new CalendarSelectionPageListCellCreator;
	m_list->setCellCreator(creator);
	m_list->setItemModel(m_model);
	connect(m_list, SIGNAL(itemClicked(const QModelIndex &)),
		this, SLOT(calendarItemSelected(const QModelIndex &)));

	connect(m_model, SIGNAL(ready()), this, SLOT(itemsReady()));
	m_model->fetch();
}

void CalendarSelectionPage::calendarItemSelected(const QModelIndex &which)
{
	dismiss();
	Q_EMIT(selected(m_model->calendarItem(which)));
}

void CalendarSelectionPage::itemsReady(void)
{
	QModelIndex group = m_model->groupClosestToNow();
	if (group.isValid()) {
		mDebug(__func__) 
		<< "Scroll(" << group.column() << "," << group.row() << ")";
		m_list->scrollTo(group, 
				 MList::PositionAtTopHint,
				 MList::NonAnimated);
	}
}
