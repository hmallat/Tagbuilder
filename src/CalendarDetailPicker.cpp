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
			    tr("No calendar entry details to select from"),
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
		mDebug(__func__) << "Saving " << detail.definitionName();
		picked.saveDetail(&detail);
	}

	dismiss();
	Q_EMIT(calendarPicked(picked));
}
