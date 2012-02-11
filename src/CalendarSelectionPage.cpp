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
#include "LabelOrList.h"

#include <MDebug>

static MAbstractCellCreator<MWidgetController> *_getCreator(void)
{
	return new CalendarSelectionPageListCellCreator;
}

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
	createCommonContent(m_model,
			    _getCreator,
			    tr("Fetching calendar items"),
			    tr("Select calendar entry"),
			    true,
			    false);

	connect(m_list, SIGNAL(itemClicked(const QModelIndex &)),
		this, SLOT(calendarItemSelected(const QModelIndex &)));

	connect(m_model, SIGNAL(ready()), this, SLOT(itemsReady()));
	if (m_model->fetch() == true) {
		setBusy();
	} else {
		m_list->setLabel(tr("Cannot retrieve calendar entries"));
	}
}

void CalendarSelectionPage::calendarItemSelected(const QModelIndex &which)
{
	dismiss();
	Q_EMIT(selected(m_model->calendarItem(which)));
}

void CalendarSelectionPage::itemsReady(void)
{
	clearBusy();

	m_list->setLabel(tr("No calendar entries to select from"));

	QModelIndex group = m_model->groupClosestToNow();
	if (group.isValid()) {
		mDebug(__func__) 
		<< "Scroll(" << group.column() << "," << group.row() << ")";
		m_list->scrollTo(group);
	}
}
