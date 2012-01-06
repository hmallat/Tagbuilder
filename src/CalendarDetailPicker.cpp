/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "CalendarDetailPicker.h"
#include "CalendarDetailPickerListCellCreator.h"
#include "CalendarDetailPickerListModel.h"
#include "LabelOrList.h"
#include "Util.h"

#include <QItemSelectionModel>
#include <QOrganizerManager>

#include <QGraphicsAnchorLayout>
#include <MAction>
#include <MLabel>
#include <MList>

#include <MDebug>

static MAbstractCellCreator<MWidgetController> *_getCreator(void)
{
	return new CalendarDetailPickerListCellCreator;
}

CalendarDetailPicker::CalendarDetailPicker(const QOrganizerItem item,
					   QGraphicsItem *parent)
	: SelectionPage(parent),
	  m_item(item),
	  m_model(new CalendarDetailPickerListModel(this))
{
	m_model->setOrganizerItem(item);
}

CalendarDetailPicker::~CalendarDetailPicker(void)
{
}

void CalendarDetailPicker::createContent(void)
{
	createCommonContent(m_model,
			    _getCreator,
			    tr("<h1>No calendar entry details to select</h1>"),
			    tr("Select calendar entry details"),
			    true,
			    true);

	connect(this, SIGNAL(done()), this, SLOT(pickingDone()));
	selectAll(m_model);
}

void CalendarDetailPicker::pickingDone(void)
{
	QOrganizerManager manager;

	QItemSelectionModel *selection = m_list->selectionModel();
	if (selection == NULL) {
		mDebug(__func__) << "No selection model. ";
		return;
	}

	/* Construct a new calendar out of the selected details 
	   and give that as the result of the pick. */
	QOrganizerItem picked;
	QString type;
	if (m_item.type() == "EventOccurrence") {
		type = "Event";
	} else if (m_item.type() == "TodoOccurrence") {
		type = "Todo";
	} else {
		type = m_item.type();
	}
	picked.setType(type);
	
	QModelIndexList list = selection->selectedIndexes();
	for (int i = 0; i < list.length(); i++) {
		QOrganizerItemDetail detail = m_model->detail(list[i]);
		/* TODO: will this cause problems, does the 
		   damn qorganizeritem make a copy or not? */
		picked.saveDetail(&detail);
	}

	dismiss();
	Q_EMIT(calendarPicked(picked));
}
